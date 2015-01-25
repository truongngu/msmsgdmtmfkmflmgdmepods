using System;
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
        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int CreategameWithHandle(IntPtr hWnd);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int KeyRequestHandle(uint key);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MouseRequestHandle(bool isDown, int x, int y, int type);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MoveCameraTo(float x, float y);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int MoveSelectedEntity(float x, float y);

        [DllImport("Game.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void GetPickingEntity(StringBuilder name,float x, float y);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ReleaseSelectedEntity();

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetHandleParent(IntPtr hWnd);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int ExitRequestHandle();

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetHandleLocaltion(int x, int y, int w, int h);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int SetWindowSizeChange(int x, int y, int w, int h);

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern int RefreshRequestHandle();

        [DllImport("Game.dll", CharSet = CharSet.Unicode, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Rungame(float deltaTime);

        [DllImport("Game.dll", CharSet = CharSet.Auto, CallingConvention = CallingConvention.Cdecl)]
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
        DispatcherTimer timer = new DispatcherTimer();

        public MainWindow()
        {
            InitializeComponent();

            propertiGrid.SelectedObject = game;
            propertiGrid.KeyUp += propertiGrid_KeyUp;
        }

        void propertiGrid_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SaveConfig();
                KeyRequestHandle(13);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
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
            string path = "../Resources/Gameplay.xml";
            game.LoadFromXml(path);
        }

        private void SaveConfig()
        {
            string path = "../Resources/Gameplay.xml";
            game.WriteToXml(path);
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
            int eglX = (int)renderedLocation.X;
            int eglY = (int)renderedLocation.Y;
            int eglW = (int)lblRender.ActualWidth;
            int eglH = (int)lblRender.ActualHeight;

            unsafe
            {
                //CreateSurface((int)this.Width, (int)this.Height, hwnd.Handle, hinstance);
                SetHandleParent(hwnd.Handle);
                SetHandleLocaltion(eglX, eglY, eglW, eglH);
                CreategameWithHandle(hwnd.Handle);
            }

          
            timer.Interval = TimeSpan.FromSeconds((double)1/60.0f);
            timer.Tick += timer_Tick;
            timer.Start();
        }

        void timer_Tick(object sender, EventArgs e)
        {
           
            double timeStep = timer.Interval.TotalSeconds;
            try
            {
                Rungame(0.03f);
                //DrawFrame(1/60.0f);
               // DrawFrame(1/60.0f);
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message, ex.TargetSite.Name);
            }
        }

        bool isMouseDown = false;
        Point prevPos = new Point();
        Point currPos = new Point();
        float CameraX = 0, CameraY = 0;
        private void lblRender_MouseUp(object sender, MouseButtonEventArgs e)
        {
            MouseRequestHandle(false, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            ReleaseSelectedEntity();
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
            txtMouseMovePos.Content = currPos.X + "," + currPos.Y;
            double prevCamX = prevPos.X * cameraWidth / viewWidth;
            double prevCamY = prevPos.Y * cameraHeight / viewHeight;
            double currCamX = currPos.X * cameraWidth / viewWidth;
            double currCamY = currPos.Y * cameraHeight / viewHeight;
            prevPos = currPos;
            if (isMouseDown)
            {
                //kiem tra chon entity thi move entity
                float deltaX = (float)(currCamX - prevCamX);
                float deltaY = (float)(currCamY - prevCamY);
                CameraX -= deltaX;
                CameraY += deltaY;
               // MoveCameraTo(CameraX, CameraY);
                MoveSelectedEntity(deltaX, -deltaY);
            }
        }

        private void lblRender_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(lblRender).X;
            int y= (int)e.GetPosition(lblRender).Y;
            txtMouseDownPos.Content = x + "," + y;
            MouseRequestHandle(true, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            isMouseDown = true;
            StringBuilder name = new StringBuilder();
            ReleaseSelectedEntity();
            GetPickingEntity(name,(int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y);
            //Console.WriteLine(name.ToString());
            //MessageBox.Show(name.ToString(), "Name");
            prevPos = e.GetPosition(lblRender);
            
        }

        private void lblRender_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Point renderedLocation = lblRender.TranslatePoint(new Point(0, 0), this);
            int x = (int)renderedLocation.X;
            int y = (int)renderedLocation.Y;
            int wid = (int)e.NewSize.Width;
            int hei = (int)e.NewSize.Height;

            SetWindowSizeChange(x, y, wid, hei);
        }

        private void Add_Sprite(object sender, MouseButtonEventArgs e)
        {
            Microsoft.Win32.OpenFileDialog dlg = new Microsoft.Win32.OpenFileDialog();
            dlg.DefaultExt = ".tga";
            dlg.Filter = "TGA Files (*.tga)|*.tga";
            Nullable<bool> result = dlg.ShowDialog();


            // Get the selected file name and display in a TextBox 
            if (result == true)
            {
                // Open document 
                string path = dlg.FileName;
                string file = System.IO.Path.GetFileName(path);
                string curpath = System.AppDomain.CurrentDomain.BaseDirectory;
                string topath=System.IO.Path.Combine("../Resources/Textures/"+file);
                CopyFile(path, topath, true);
                string path2Read = "Resources/Textures/" + file;

                EntityInfor infor = new EntityInfor(path2Read);
                game.Add(infor, "../Resources/Gameplay.xml");
                //game.LoadFromXml("../Resources/Gameplay.xml");
                KeyRequestHandle(13);
            }
        }

        private void Remove_Sprite(object sender, MouseButtonEventArgs e)
        {

        }

        #region Method
        //Kiem tra xem da ton tai file texture trong Resource chua
        //fromPath: chep tu path nay
        //toPath: den path nay
        //replace: true thi replace file ton tai
        void CopyFile(string fromPath,string toPath,bool replace)
        {
            System.IO.File.Copy(fromPath, toPath, true);
        }

       
        #endregion
    }
}
