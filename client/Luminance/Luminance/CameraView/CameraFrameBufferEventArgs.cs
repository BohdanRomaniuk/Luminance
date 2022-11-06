namespace Luminance.CameraView
{
    public class CameraFrameBufferEventArgs : EventArgs
    {
        public CameraFrameBufferEventArgs(PixelBufferHolder pixelBufferHolder) : base()
            => Data = pixelBufferHolder;

        public readonly PixelBufferHolder Data;
    }
}
