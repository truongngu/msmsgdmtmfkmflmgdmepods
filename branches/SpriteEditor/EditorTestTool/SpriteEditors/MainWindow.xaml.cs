﻿using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.IO;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Interop;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Threading;

namespace SpriteEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        #region DLLImport
        // Use DllImport to import the Win32 MessageBox function.
        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CreategameWithHandle(IntPtr hWnd);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int KeyRequestHandle(uint key);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MouseRequestHandle(bool isDown, int x, int y, int type);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MoveCameraTo(float x, float y);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetHandleParent(IntPtr hWnd);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExitRequestHandle();

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetHandleLocaltion(int x, int y, int w, int h);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetWindowSizeChange(int x, int y, int w, int h);

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RefreshRequestHandle();

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int Rungame();

        [DllImport("Win32\\Game.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExecuteGame(StringBuilder method, object param);

        //call freelibrary
        [DllImport("kernel32.dll", EntryPoint = "LoadLibrary")]
        static extern int LoadLibrary(
            [MarshalAs(UnmanagedType.LPStr)] string lpLibFileName);

        [DllImport("kernel32.dll", EntryPoint = "GetProcAddress")]
        static extern IntPtr GetProcAddress(int hModule,
            [MarshalAs(UnmanagedType.LPStr)] string lpProcName);

        [DllImport("kernel32.dll", EntryPoint = "FreeLibrary")]
        static extern bool FreeLibrary(int hModule);
        #endregion

        GameConfig game = new GameConfig();

        public MainWindow()
        {
            InitializeComponent();

            propertiGrid.SelectedObject = game;
            
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                //BackgroundWorker worker = new BackgroundWorker();
                //HandleInfor infor = new HandleInfor();
                //infor.x = eglX;
                //infor.y = eglY;
                //infor.w = eglW;
                //infor.h = eglH;
                //infor.hWnd = hwnd.Handle;
                //infor.hinstance = hinstance;
                //worker.DoWork += worker_DoWork;
                //worker.WorkerReportsProgress = true;
                //worker.RunWorkerAsync(infor);
                //worker.ProgressChanged += worker_ProgressChanged;

                int hModule = LoadLibrary("Game.dll");
                if (hModule == 0) throw new Exception("Không mở được Game");
                IntPtr intPtr = GetProcAddress(hModule, "Rungame");
                if (intPtr == IntPtr.Zero) throw new Exception("Không mở được phương thức trong Game");
                //read config file
                LoadConfig();

            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message);
                this.Close();
            }
            ((Window)sender).InvalidateVisual();
            btnUpdate.InvalidateVisual();
        }

        private void LoadConfig()
        {
            string path = "../Resources/Gameplay.txt";
            string[] allConfig = File.ReadAllLines(path);
            txtConfig.Text = "";
            for (int i = 0; i < allConfig.Length; i++)
            {
                txtConfig.Text += allConfig[i] + Environment.NewLine;
            }
        }

        private void SaveConfig()
        {
            string path = "../Resources/Gameplay.txt";
            string allConfig = txtConfig.Text;

            File.WriteAllText(path, allConfig);
        }

        void worker_ProgressChanged(object sender, ProgressChangedEventArgs e)
        {

        }

        void worker_DoWork(object sender, DoWorkEventArgs e)
        {
            HandleInfor infor = (HandleInfor)e.Argument;
            unsafe
            {
                SetHandleParent(infor.hWnd);
                SetHandleLocaltion(infor.x, infor.y, infor.w, infor.h);
                CreategameWithHandle(infor.hWnd);
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            try
            {
                ExitRequestHandle();
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message);
            }
        }

        private void Button_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                //RefreshRequestHandle();
                SaveConfig();
                KeyRequestHandle(13);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message);
            }
        }

        private void Label_MouseUp(object sender, MouseButtonEventArgs e)
        {
            ExitRequestHandle();
            WindowInteropHelper windowHwnd = new WindowInteropHelper(this);
            IntPtr hWnd = windowHwnd.Handle;
            HwndSource hwnd = (HwndSource)HwndSource.FromVisual(lblRender);
            IntPtr hinstance = Marshal.GetHINSTANCE(typeof(App).Module);
            Point renderedLocation = lblRender.TranslatePoint(new Point(0, 0), this);
            int eglX = (int)renderedLocation.X + 5;
            int eglY = (int)renderedLocation.Y + 5;
            int eglW = (int)lblRender.ActualWidth - 10;
            int eglH = (int)lblRender.ActualHeight - 10;

            unsafe
            {
                //CreateSurface((int)this.Width, (int)this.Height, hwnd.Handle, hinstance);
                SetHandleParent(hwnd.Handle);
                SetHandleLocaltion(eglX, eglY, eglW, eglH);
                CreategameWithHandle(hwnd.Handle);
            }

            DispatcherTimer timer = new DispatcherTimer();
            timer.Interval = TimeSpan.FromMilliseconds((double)1000 / 60);
            timer.Tick += timer_Tick;
            timer.Start();
        }

        void timer_Tick(object sender, EventArgs e)
        {
            try
            {
                Rungame();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.TargetSite.Name);
            }
            //StringBuilder sb = new StringBuilder(10);
            //sb.AppendLine("Rungame");
            //ExecuteGame(sb, 1);
        }

        bool isMouseDown = false;
        Point prevPos = new Point();
        Point currPos = new Point();
        float CameraX = 0, CameraY = 0;
        private void lblRender_MouseUp(object sender, MouseButtonEventArgs e)
        {
            MouseRequestHandle(false, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            isMouseDown = false;
        }

        private void lblRender_MouseMove(object sender, MouseEventArgs e)
        {
            //MouseRequestHandle(isMouseDown, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            //width of camera is 25
            //height of camera is 34
            double cameraWidth = 25;
            double cameraHeight = 34;
            double viewWidth = lblRender.ActualWidth;
            double viewHeight = lblRender.ActualHeight;
            currPos = e.GetPosition(lblRender);
            double prevCamX = prevPos.X * cameraWidth / viewWidth;
            double prevCamY = prevPos.Y * cameraHeight / viewHeight;
            double currCamX = currPos.X * cameraWidth / viewWidth;
            double currCamY = currPos.Y * cameraHeight / viewHeight;
            prevPos = currPos;
            if (isMouseDown)
            {

                float deltaX = (float)(currCamX - prevCamX);
                float deltaY = (float)(currCamY - prevCamY);
                CameraX -= deltaX;
                CameraY += deltaY;
                MoveCameraTo(CameraX, CameraY);
            }
        }

        private void lblRender_MouseDown(object sender, MouseButtonEventArgs e)
        {
            MouseRequestHandle(true, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            isMouseDown = true;
            prevPos = e.GetPosition(lblRender);
        }

        private void lblRender_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Point renderedLocation = lblRender.TranslatePoint(new Point(0, 0), this);
            int x = (int)renderedLocation.X + 5;
            int y = (int)renderedLocation.Y + 5;
            int wid = (int)e.NewSize.Width - 10;
            int hei = (int)e.NewSize.Height - 10;

            SetWindowSizeChange(x, y, wid, hei);
        }
    }
}
