using Luminance.Client.Pages;
using Luminance.Client.ViewModels;
using Luminance.Client.ViewModels.Mapping;

namespace Luminance.Client.Helpers
{
    public static class PageFactory
    {
        private static readonly Dictionary<Type, Type> RegisteredPages = new Dictionary<Type, Type>();

        public static void Init()
        {
            Register<CreateMappingPage, CreateMappingViewModel>();
            Register<SettingsPage, SettingsViewModel>();
            Register<MappingResultPage, MappingResultViewModel>();
            Register<CreateEffectPage, CreateEffectViewModel>();
            Register<GalleryPage, GalleryViewModel>();
        }

        public static void Register<T, V>() where V : BaseViewModel =>
            RegisteredPages[typeof(V)] = typeof(T);

        public static Page? CreatePage<T>() where T : BaseViewModel
            => CreatePage(typeof(T));

        public static Page? CreatePage(Type viewModelType)
            => CreatePage(CreateViewModel(viewModelType));

        public static Page? CreatePage<T>(params object[] args) where T : BaseViewModel
            => CreatePage(CreateViewModel(typeof(T), args));

        public static Page? CreatePage(BaseViewModel? viewModel)
        {
            if (viewModel == null)
            {
                return null;
            }

            try
            {
                var vmType = viewModel.GetType();
                if (!RegisteredPages.ContainsKey(vmType))
                {
                    throw new InvalidOperationException($"Unknown View for ViewModel: {vmType?.Name}");
                }

                var viewType = RegisteredPages[vmType];
                var page = (Page?)Activator.CreateInstance(viewType);
                if (page == null)
                {
                    return null;
                }

                page.BindingContext = viewModel;
                viewModel.ContentPage = page;

                return page;
            }
            catch (Exception ex)
            {
                ex.Report();

                throw;
            }
        }

        public static BaseViewModel? CreateViewModel(Type viewModelType, params object[] args)
            => Activator.CreateInstance(viewModelType, args) as BaseViewModel;
    }
}
