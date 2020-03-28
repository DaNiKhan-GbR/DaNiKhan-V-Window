#pragma once

// TODO Comments for all the constants

namespace dnkvw
{
    namespace constant
    {
        namespace capture
        {
            constexpr double targetFps = 30;
            constexpr double targetWidth = 640;
            constexpr double targetHeight = 480;
        }

        namespace frustum
        {
            constexpr float eyeAvgFactor = 0.25f;
        }

        namespace calib
        {
            constexpr int targetFaceCount = 10;
            constexpr int maxFaceTries = 50;
        }
    }
}
