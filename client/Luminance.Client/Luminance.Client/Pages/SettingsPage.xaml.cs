using Luminance.Client.ViewModels;

namespace Luminance.Client.Pages;

public partial class SettingsPage : ContentPage
{
	public SettingsPage()
	{
		InitializeComponent();
        BindingContext = new SettingsViewModel();
    }
}