﻿namespace Luminance.CameraView
{
    public interface ICameraFrameAnalyzer
    {
        void FrameReady(CameraFrameBufferEventArgs args);
    }

    public interface ICameraView : IView, ICameraFrameAnalyzer
    {
        CameraLocation CameraLocation { get; set; }

        //CameraMode Mode { get; set; }

        void AutoFocus();

        void Focus(Point point);

        bool IsTorchOn { get; set; }
    }
}
