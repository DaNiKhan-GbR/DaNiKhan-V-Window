#include "thrdWinCalc.hpp"

using dnkvw::CThreadedWindowCalculator;

CThreadedWindowCalculator::~CThreadedWindowCalculator()
{
    if (m_threadRunning.load())
    {
        this->stop();
    }

    if (m_tracker)
    {
        m_tracker->cleanup();
        delete m_tracker;
    }
}

void CThreadedWindowCalculator::start(int cameraId)
{
    if (!m_threadRunning.load())
    {
        m_threadRunning = true;
        m_thread = std::thread([this, cameraId]{ this->processingLoop(cameraId); });
    }
}

void CThreadedWindowCalculator::stop()
{
    this->stopAsync();
    
    if (m_thread.joinable())
    {
        m_thread.join();
    }
}

void CThreadedWindowCalculator::stopAsync()
{
    m_signal_stop = true;
}

bool CThreadedWindowCalculator::isRunning()
{
    return m_threadRunning.load();
}

dnkvw::CWindowResult CThreadedWindowCalculator::obtainLastResult()
{
    return m_lastResult.load();
}

void CThreadedWindowCalculator::storeSettings(dnkvw::CWindowSettings settings)
{
    m_settings = settings;
}

void CThreadedWindowCalculator::signalCalibrate()
{
    m_signal_calibrate = true;
}

bool CThreadedWindowCalculator::selectTracker(dnkvw::ITracker* tracker)
{
    if (tracker->init())
    {
        if (m_tracker)
        {
            m_tracker->cleanup();
            delete m_tracker;
        }

        m_tracker = tracker;
        return true;
    }
    else
    {
        delete tracker;
        return false;
    }
}

void CThreadedWindowCalculator::faceToEye(const cv::Rect& face, const float frameWidth, const float frameHeight, dnkvw::Vec3& eye)
{
    // TODO too hard for an good approx.
    eye[0] = ((float)(face.x + face.width / 2.0f)) / frameWidth * 2.0f - 1.0f;
    eye[1] = ((float)(face.y + face.height / 4.0f)) / frameHeight * 2.0f - 1.0f; 
    eye[2] = 100.0f / (float)face.width;
}

void CThreadedWindowCalculator::initVideoCapture(int cameraId)
{
    bool success;
#ifdef _WIN32
    success = m_videoCapture.open(cameraId, cv::CAP_MSMF);
#else
    success = m_videoCapture.open(cameraId);
#endif

    if (success)
    {
        m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, dnkvw::constant::targetWidth);
        m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, dnkvw::constant::targetHeight);
        m_videoCapture.set(cv::CAP_PROP_FPS, dnkvw::constant::targetFps);

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH) - dnkvw::constant::targetWidth) > 0.1f || 
            ::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT) - dnkvw::constant::targetHeight) > 0.1f) 
        {
            std::cout << "WARNING: Camera resolution doesn't match target resolution of " << dnkvw::constant::targetWidth 
                    << "x" << dnkvw::constant::targetHeight << ". Tracking results may be wrong." << std::endl;
        }

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FPS) - dnkvw::constant::targetFps) > 0.1f) 
        {
            std::cout << "WARNING: Camera framerate doesn't match target framerate of " << dnkvw::constant::targetFps 
                    << "fps. Tracking results may be wrong." << std::endl;
        }
    }
}

void CThreadedWindowCalculator::processingLoop(int cameraId)
{
    // Init
    initVideoCapture(cameraId);

    // Processing Loop
    while (!m_signal_stop)
    {
        if (m_signal_calibrate)
        {
            this->calibrate();
            m_signal_calibrate = false;
        }
        else
        {
            this->calcWindow();
        }

        // std::this_thread::yield(); // TODO check if relevant?
    }

    // Cleanup
    m_videoCapture.release();
    m_threadRunning = false;
}

void CThreadedWindowCalculator::calibrate()
{
    // TODO as a real constant
    constexpr int targetFaceCount = 10;
    constexpr int maxFaceTries = 50;

    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    std::vector<cv::Rect> faces;

    int tryCount = 0;
    while (faces.size() < targetFaceCount && tryCount < maxFaceTries)
    {
        tryCount++;
        m_videoCapture >> frame;
        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            faces.push_back(optFace.value());
        }
    }

    if (faces.size() >= targetFaceCount / 2)
    {
        Vec3 eyeAvg;

        for (auto const& face : faces)
        {
            Vec3 eye;
            faceToEye(face, (float)frame.cols, (float)frame.rows, eye);
            eyeAvg += eye;
        }

        eyeAvg /= (float) faces.size();
        
        m_eyeOffset = -eyeAvg;
        m_eyeOffset[2] = eyeAvg[2]; // TODO Mal schauen, aber richtig hart
    }
}

void CThreadedWindowCalculator::calcWindow()
{
    m_fpsTimer.start();
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    m_videoCapture >> frame;
    std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

    if (optFace)
    {
        CWindowSettings settings = m_settings.load();
        cv::Rect face = optFace.value();

        Vec3 eye;
        faceToEye(face, (float)frame.cols, (float)frame.rows, eye);
        eye += m_eyeOffset;

        Vec3 pa(-settings.aspect, -1, 0);
        Vec3 pb( settings.aspect, -1, 0);
        Vec3 pc(-settings.aspect,  1, 0);
        Vec3 pe(-eye[0], -eye[1], eye[2]);

        Vec3 vr = (pb - pa).norm();
        Vec3 vu = (pc - pa).norm();
        Vec3 vn = vr.cross(vu).norm();

        Vec3 va = pa - pe;
        Vec3 vb = pb - pe;
        Vec3 vc = pc - pe;

        float d = -(vn * va);
        float nearOverD = settings.near / d;

        float l = (vr * va) * nearOverD;
        float r = (vr * vb) * nearOverD;
        float b = (vu * va) * nearOverD;
        float t = (vu * vc) * nearOverD;

        // Set result
        CWindowResult newResult;
        newResult.fps = m_fpsTimer.stop(); // TODO Capture fps for frame dropping / automatic quality adjustments
        newResult.left = l;
        newResult.right = r;
        newResult.bottom = b;
        newResult.top = t;

        m_lastResult = newResult;
    }
}
