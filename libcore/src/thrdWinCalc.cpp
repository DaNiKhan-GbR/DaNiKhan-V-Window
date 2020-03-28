#include "thrdWinCalc.hpp"
#include "constants.hpp"

using dnkvw::CThreadedWindowCalculator;

CThreadedWindowCalculator::~CThreadedWindowCalculator()
{
    if (m_threadRunning.load())
    {
        this->stop();
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

bool CThreadedWindowCalculator::selectTracker(std::unique_ptr<dnkvw::ITracker> tracker)
{
    if (tracker->init())
    {
        m_tracker = std::move(tracker);
        return true;
    }

    return false;
}

void CThreadedWindowCalculator::debugCameraInput(int cameraId)
{
    initVideoCapture(cameraId);

    if (!m_videoCapture.isOpened())
    {
        return;
    }

    const std::string WINDOW_NAME = "Dnkvw Debug Input";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);
    cv::Mat frame;

    while (true)
    {
        m_fpsTimer.start();

        m_videoCapture >> frame;

        cv::flip(frame, frame, 1);

        float fps = m_fpsTimer.stop();

        char buffer[40] = { 0 };
        snprintf(buffer, 40, "FPS: %.1f", fps);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
    cv::waitKey(1); // Force closing window on mac

    m_videoCapture.release();
}

void CThreadedWindowCalculator::debugCameraFace(int cameraId)
{
    initVideoCapture(cameraId);

    if (!m_videoCapture.isOpened() || !m_tracker)
    {
        return;
    }
    
    const std::string WINDOW_NAME = "Dnkvw Debug Facedetection";

    cv::namedWindow(WINDOW_NAME, cv::WINDOW_AUTOSIZE);

    cv::Mat frame;

    while (true)
    {
        m_fpsTimer.start();

        m_videoCapture >> frame;

        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            cv::rectangle(frame, optFace.value(), cv::Scalar(0, 255, 0), 2);
        }

        cv::flip(frame, frame, 1);

        float fps = m_fpsTimer.stop();

        char buffer[40] = { 0 };
        snprintf(buffer, 40, "FPS: %.1f", fps);
        cv::putText(frame, buffer, cv::Point(20, 40), cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(0, 255, 0));

        cv::imshow(WINDOW_NAME, frame);

        if (cv::waitKey(1) >= 0)
        {
            break;
        }
    }

    cv::destroyWindow(WINDOW_NAME);
    cv::waitKey(1); // Force closing window on mac

    m_videoCapture.release();
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
        m_videoCapture.set(cv::CAP_PROP_FRAME_WIDTH, constant::capture::targetWidth);
        m_videoCapture.set(cv::CAP_PROP_FRAME_HEIGHT, constant::capture::targetHeight);
        m_videoCapture.set(cv::CAP_PROP_FPS, constant::capture::targetFps);

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_WIDTH) - constant::capture::targetWidth) > 0.1f || 
            ::fabs(m_videoCapture.get(cv::CAP_PROP_FRAME_HEIGHT) - constant::capture::targetHeight) > 0.1f) 
        {
            std::cout << "WARNING: Camera resolution doesn't match target resolution of " << constant::capture::targetWidth 
                    << "x" << constant::capture::targetHeight << ". Tracking results may be wrong." << std::endl;
        }

        if (::fabs(m_videoCapture.get(cv::CAP_PROP_FPS) - constant::capture::targetFps) > 0.1f) 
        {
            std::cout << "WARNING: Camera framerate doesn't match target framerate of " << constant::capture::targetFps 
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
    }

    // Cleanup
    m_videoCapture.release();
    m_threadRunning = false;
}

void CThreadedWindowCalculator::calibrate()
{
    if (!m_videoCapture.isOpened())
    {
        return;
    }

    cv::Mat frame;
    std::vector<cv::Rect> faces;

    int tryCount = 0;
    while (faces.size() < constant::calib::targetFaceCount && tryCount < constant::calib::maxFaceTries)
    {
        tryCount++;
        m_videoCapture >> frame;
        std::optional<cv::Rect> optFace = m_tracker->trackFrame(frame);

        if (optFace)
        {
            faces.push_back(optFace.value());
        }
    }

    if (faces.size() >= constant::calib::targetFaceCount / 2)
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

        constexpr float eyeAvgFactor = constant::frustum::eyeAvgFactor;
        m_eyeAvg = m_eyeAvg * (1.0f - eyeAvgFactor) + eye * eyeAvgFactor;

        Vec3 pa(-settings.aspect, -1, 0);
        Vec3 pb( settings.aspect, -1, 0);
        Vec3 pc(-settings.aspect,  1, 0);
        Vec3 pe(-m_eyeAvg[0], -m_eyeAvg[1], m_eyeAvg[2]);

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
