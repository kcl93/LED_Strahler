﻿namespace LED_Strahler_GUI
{
    using System;
    using System.Collections.Generic;
    using System.Linq;
    using System.Windows;
    using System.Windows.Controls;
    using System.ComponentModel;
    using System.Windows.Threading;
    using System.Windows.Input;

    public partial class LEDStrahlerBackend
    {

        private readonly MainWindow GUI = null;

        private readonly DispatcherTimer Timer = new DispatcherTimer(DispatcherPriority.Render);

        private readonly List<Key> PressedKeys = new List<Key>();

        #region Constructor

        public LEDStrahlerBackend(MainWindow _gui)
        {
            GUI = _gui;

            //Link shutdown event
            this.GUI.Closing += new CancelEventHandler(this.GUIClosing);

            //Get available serial ports
            UpdateCOMPortList();

            //Connect serial port to all GroupControls
            LEDStrahlerSerial.StartDispatcher();

            //Link Buttons
            this.GUI.ConnectRefreshButton.Click += this.ConnectRefreshButtonClick;
            this.GUI.GetTemperatureButton.Click += this.GetTemperatureButtonClick;

            //Link config tab
            this.GUI.GUITabs.SelectionChanged += this.TabSelectionChanged;

            //Link key up/down
            this.GUI.Control.KeyDown += this.Control_KeyDown;
            this.GUI.Control.KeyUp += this.Control_KeyUp;

            //Empty list of pressed keys
            this.PressedKeys.Clear();

            //Start timer for LJ stuffs
            this.Timer.Interval = new TimeSpan(100000); //10ms interval
            this.Timer.Tick += new EventHandler(TimerTick);
            this.Timer.Start();
        }

        #endregion

        #region GUI triggered methods

        public void TabSelectionChanged(object sender, SelectionChangedEventArgs e)
        {
            if (this.GUI.Config.IsSelected == true)
            {
                //Clear list of pressed buttons
                this.PressedKeys.Clear();

                //Update COM port list
                this.UpdateCOMPortList();
            }
        }

        public void GUIClosing(object sender, CancelEventArgs e)
        {
            LEDStrahlerSerial.ShutdownDispatcher();
        }

        public void ConnectRefreshButtonClick(object sender, RoutedEventArgs e)
        {
            //Open serial port
            if(this.GUI.ComPort.Text != null)
            {
                LEDStrahlerSerial.Open(this.GUI.ComPort.Text);
            }

            //Ping for devices
            LEDStrahlerSerial.PingRequest(out List<uint> UUIDs);

            //Setup new device list
            List<LEDStrahler> DeviceList = new List<LEDStrahler>();
            foreach (uint UUID in UUIDs)
            {
                DeviceList.Add(new LEDStrahler(UUID));
            }
            this.GUI.DeviceList = DeviceList;

            //Read all temperatures once
            foreach (LEDStrahler Strahler in this.GUI.DeviceList)
            {
                LEDStrahlerSerial.GetTemperature(Strahler);
            }

            this.GUI.DeviceGrid.IsEnabled = true;
        }


        public void GetTemperatureButtonClick(object sender, RoutedEventArgs e)
        {
            foreach (LEDStrahler Strahler in this.GUI.DeviceList)
            {
                LEDStrahlerSerial.GetTemperature(Strahler);
            }
        }


        public void Control_KeyDown(object sender, KeyEventArgs e)
        {
            if(this.PressedKeys.Contains(e.Key) == false)
            {
                this.PressedKeys.Add(e.Key);
            }
        }


        public void Control_KeyUp(object sender, KeyEventArgs e)
        {
            if(this.PressedKeys.Contains(e.Key) != false)
            {
                this.PressedKeys.Remove(e.Key);
            }
        }

        #endregion

        #region Internal methods and helper functions

        private void UpdateCOMPortList()
        {
            var PortList = LEDStrahlerSerial.ListPorts();

            PortList.Sort();

            if(PortList.SequenceEqual(this.GUI.ComPortList) == false)
            {
                string SelectedPort = this.GUI.ComPort.Text;
                if((PortList.Count != 0) && (PortList.Contains(SelectedPort)))
                {
                    this.GUI.ComPortList = PortList;
                    this.GUI.ComPort.SelectedIndex = PortList.IndexOf(SelectedPort);
                }
                else
                {
                    this.GUI.ComPortList = PortList;
                    if(PortList.Count != 0)
                    {
                        this.GUI.ComPort.SelectedIndex = 0;
                    }
                }
            }
        }

        private List<LEDStrahler> ListMusicStrahler()
        {
            List<LEDStrahler> LJStrahler = new List<LEDStrahler>();

            foreach (LEDGroupControl LEDGroup in this.GUI.GroupControls)
            {
                if (LEDGroup.Backend.MusicControl == true)
                {
                    if (LEDGroup.GroupID == 0)
                    {
                        return this.GUI.DeviceList; //Broadcast group includes all LEDStrahler
                    }
                    foreach (LEDStrahler Strahler in this.GUI.DeviceList)
                    {
                        if (Strahler.Group == LEDGroup.GroupID)
                        {
                            LJStrahler.Add(Strahler);
                        }
                    }
                }
            }

            return LJStrahler;
        }

        private List<LEDStrahler> ListCandleStrahler()
        {
            List<LEDStrahler> LJStrahler = new List<LEDStrahler>();

            foreach (LEDGroupControl LEDGroup in this.GUI.GroupControls)
            {
                if (LEDGroup.Backend.CandleMode == true)
                {
                    if (LEDGroup.GroupID == 0)
                    {
                        return this.GUI.DeviceList; //Broadcast group includes all LEDStrahler
                    }
                    foreach (LEDStrahler Strahler in this.GUI.DeviceList)
                    {
                        if (Strahler.Group == LEDGroup.GroupID)
                        {
                            LJStrahler.Add(Strahler);
                        }
                    }
                }
            }

            return LJStrahler;
        }

        public void TimerTick(object sender, EventArgs e)
        {
            List<LEDStrahler> StrahlerListe;

            //Handle LJ stuff
            StrahlerListe = ListLJStrahler();
            if(StrahlerListe.Count > 0)
            {
                HandleLJMode(StrahlerListe);
            }
            else
            {
                this.LJUpdateLEDs = false; //Ensure that LEDs are not changing when switching to LJmode
            }

            //Handle music control stuff
            StrahlerListe = ListMusicStrahler();
            if (StrahlerListe.Count > 0)
            {

            }

            //Handle candle mode stuff
            StrahlerListe = ListCandleStrahler();
            if (StrahlerListe.Count > 0)
            {

            }
        }

        #endregion
    }
}
