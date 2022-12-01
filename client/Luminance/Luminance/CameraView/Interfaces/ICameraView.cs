namespace Luminance.CameraView
{
   

    public interface ICameraView : IView, ICameraFrameAnalyzer
    {
        CameraLocation CameraLocation { get; set; }

        void AutoFocus();

        void Focus(Point point);

        void Shutter();

        void OnMediaCaptured(Stream imageData);

        void OnMediaCapturedFailed();

        bool IsTorchOn { get; set; }
    }
}
