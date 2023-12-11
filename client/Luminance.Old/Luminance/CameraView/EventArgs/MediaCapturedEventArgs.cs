namespace Luminance.CameraView
{
    public class MediaCapturedEventArgs : EventArgs
    {
        public Stream Media { get; }

        public MediaCapturedEventArgs(Stream stream)
        {
            Media = stream;
        }
    }
}
