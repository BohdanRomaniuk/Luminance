namespace Luminance.CameraView
{
    public interface ICameraFrameAnalyzer
    {
        void FrameReady(CameraFrameBufferEventArgs args);
    }
}
