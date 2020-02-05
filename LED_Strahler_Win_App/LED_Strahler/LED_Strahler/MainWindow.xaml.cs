using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.ComponentModel;
using System.Runtime.CompilerServices;

namespace LED_Strahler_GUI
{ 
    /// <summary>
    /// Interaktionslogik für MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window, INotifyPropertyChanged
    {
        #region Property changed
            public event PropertyChangedEventHandler PropertyChanged;

            // This method is called by the Set accessor of each property.  
            // The CallerMemberName attribute that is applied to the optional propertyName  
            // parameter causes the property name of the caller to be substituted as an argument.  
            private void NotifyPropertyChanged([CallerMemberName] String propertyName = "")
            {
                PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
            }
        #endregion

        private LEDStrahlerBackend Backend = null;

        #region Properties

        private List<LEDStrahler> _DeviceList = new List<LEDStrahler>();
        public List<LEDStrahler> DeviceList
        {
            get { return _DeviceList; }
            set { _DeviceList = value; NotifyPropertyChanged(); }
        }

        private List<string> _ComPortList = new List<string>();
        public List<string> ComPortList
        {
            get { return _ComPortList; }
            set { _ComPortList = value; NotifyPropertyChanged(); }
        }

        #endregion

        #region Constructor

        public MainWindow()
        {
            InitializeComponent();

            //Create Backend
            this.Backend = new LEDStrahlerBackend(this);
        }

        #endregion
    }


    public class AvailableGroups
    {
        public List<string> Groups { get; set; } = new List<string>()
        {
            "Broadcast",
            "Group 1",
            "Group 2",
            "Group 3",
            "Group 4",
            "Group 5"
        };
    }


    public class DoubleToStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((double)value).ToString("F1") + "°C";
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return 0.0;
        }
    }


    public class UintToHexStringConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return ((uint)value).ToString("X8");
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            return 0;
        }
    }


    public class IntToGroupConverter : IValueConverter
    {
        public object Convert(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            var Group = new AvailableGroups();
            return Group.Groups[(int)value];
        }

        public object ConvertBack(object value, Type targetType, object parameter, System.Globalization.CultureInfo culture)
        {
            var Group = new AvailableGroups();
            return Group.Groups.IndexOf(value.ToString());
        }
    }
}
