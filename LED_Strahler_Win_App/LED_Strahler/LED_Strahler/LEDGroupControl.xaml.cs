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

        public byte GroupID 
        { 
            get { return this.Backend.GroupID; }
            set { this.Backend.GroupID = value; }
        }

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
