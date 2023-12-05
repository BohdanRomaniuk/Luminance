using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

using Xamarin.Forms;
using Xamarin.Forms.Xaml;

namespace Luminance.Controls
{
    public partial class TitledLabel : Grid
    {
        public static readonly BindableProperty TitleProperty =
                BindableProperty.Create(nameof(Title), typeof(string), typeof(TitledLabel), string.Empty);

        public static readonly BindableProperty TextProperty =
                BindableProperty.Create(nameof(Text), typeof(string), typeof(TitledLabel), string.Empty);

        public string Title
        {
            get => (bool)GetValue(TitleProperty);
            set => SetValue(TitleProperty, value);
        }

        public string Text
        {
            get => (bool)GetValue(TextProperty);
            set => SetValue(TextProperty, value);
        }

        public TitledLabel()
        {
            InitializeComponent();
        }
    }
}