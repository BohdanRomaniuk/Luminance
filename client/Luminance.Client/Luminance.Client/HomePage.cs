using Luminance.Client.ViewModels;
using Luminance.Client.ViewModels.Mapping;

namespace Luminance.Client;

public class HomePage : TabbedPage
{
    private readonly NavigationPage _settingsNavPage;
    private readonly NavigationPage _mappingNavPage;
    private readonly NavigationPage _galleryNavPage;
    private readonly NavigationPage _createEffectNavPage;

    public HomePage()
    {
        _settingsNavPage = CreateNavPage<SettingsViewModel>("settings", "������������");
        _mappingNavPage = CreateNavPage<MappingResultViewModel>("mapping", "����������");
        _galleryNavPage = CreateNavPage<GalleryViewModel>("gallery", "�������");
        _createEffectNavPage = CreateNavPage<CreateEffectViewModel>("magic_wand", "��������");


        Children.Add(_settingsNavPage);
        Children.Add(_mappingNavPage);
        Children.Add(_galleryNavPage);
        Children.Add(_createEffectNavPage);
    }

    private NavigationPage CreateNavPage<T>(string imageUrl, string title) where T : BaseViewModel
    {
        var page = BaseViewModel.CreatePage<T>();

        var navigationPage = new NavigationPage(page)
        {
            IconImageSource = (FileImageSource)ImageSource.FromFile(imageUrl),
            Title = title,
        };

        return navigationPage;
    }
}