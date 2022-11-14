
using Emgu.CV.Structure;
using Emgu.CV;
using Luminance.CameraView;
using System.Drawing;

namespace Luminance.Pages;

public partial class SettingsPage : ContentPage
{
    public SettingsPage()
    {
        InitializeComponent();
    }

    private void Button_Clicked(object sender, EventArgs e)
    {
        try
        {
            CvInvokeAndroid.Init();

            var source = new Image<Gray, byte>(120, 120);
        }
        catch(Exception ex)
        {

        }
    }

    private void Slider_ValueChanged(System.Object sender, Microsoft.Maui.Controls.ValueChangedEventArgs e)
    {
        
    }

    private void Slider_ValueChanged_1(System.Object sender, Microsoft.Maui.Controls.ValueChangedEventArgs e)
    {

    }

    private void Slider_ValueChanged_2(System.Object sender, Microsoft.Maui.Controls.ValueChangedEventArgs e)
    {

    }

    
}