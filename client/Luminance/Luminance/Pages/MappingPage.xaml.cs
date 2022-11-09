using Luminance.CameraView;

namespace Luminance.Pages;

public partial class MappingPage : ContentPage
{
	public MappingPage()
	{
		InitializeComponent();
	}

    private void CameraView_FrameReady(object sender, CameraView.CameraFrameBufferEventArgs e)
    {

    }

    void SwitchCameraButton_Clicked(object sender, EventArgs e)
    {
        camera.CameraLocation = camera.CameraLocation == CameraLocation.Rear ? CameraLocation.Front : CameraLocation.Rear;
    }

    void TorchButton_Clicked(object sender, EventArgs e)
    {
        camera.IsTorchOn = !camera.IsTorchOn;
    }
}