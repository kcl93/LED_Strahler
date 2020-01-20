using System.Windows.Controls;

namespace LED_Strahler_GUI
{
    /// <summary>
    /// Interaktionslogik für LEDGroupControl.xaml
    /// </summary>
    public partial class LEDGroupControl : UserControl
    {
        #region Properties

        public LEDGroupControl_Backend Backend { get; set; } = new LEDGroupControl_Backend();

        public byte GroupID { get; set; } = 0;

        #endregion

        #region Constructor

        public LEDGroupControl()
        {
            InitializeComponent();

            this.Backend.LinkGUI(this);
        }

        #endregion
    }
}
