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
using Game2DDemo;

namespace SpriteEditor
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    /// 
    public partial class MainWindow : Window
    {
        GameInstance game2D;
        GameConfig game = new GameConfig();
        DispatcherTimer timer = new DispatcherTimer();

        CloneInfor selectedClone=null;
        
        public MainWindow()
        {
            InitializeComponent();
            Global.propertitiesGrid = this.propertiGrid;
            propertiGrid.SelectedObject = game;
            propertiGrid.KeyUp += propertiGrid_KeyUp;
        }

        void propertiGrid_KeyUp(object sender, KeyEventArgs e)
        {
            if (e.Key == Key.Enter)
            {
                SaveConfig();
                game2D.KeyDownRequestHandle(13);
            }
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            try
            {
                game2D = new GameInstance();
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
                game2D.SetHandleLocaltion(infor.X, infor.Y, infor.W, infor.H);
                game2D.CreategameWithHandle(infor.hWnd.ToPointer());
            }
        }

        private void Window_Closing(object sender, System.ComponentModel.CancelEventArgs e)
        {
            try
            {
                game2D.ExitRequestHandle();
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
                Global.RefreshEditorGrid();
                SaveConfig();
                game2D.KeyDownRequestHandle(13);
            }
            catch (Exception ex)
            {
                System.Windows.MessageBox.Show(ex.Message);
            }
        }

        private void Label_MouseUp(object sender, MouseButtonEventArgs e)
        {
            game2D.ExitRequestHandle();
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
                game2D.SetHandleLocaltion(eglX, eglY, eglW, eglH);
                game2D.CreategameWithHandle(hwnd.Handle.ToPointer());
            }

          
            timer.Interval = TimeSpan.FromSeconds((double)1/60.0f);
            timer.Tick += timer_Tick;
            timer.Start();
        }

        int ddtimer = 0;
        void timer_Tick(object sender, EventArgs e)
        {
            double timeStep = timer.Interval.TotalSeconds;
            try
            {
                game2D.Rungame(0.03f);
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
            if (selectedClone != null)
            {
                string path = "../Resources/Gameplay.xml";
                game.EditClone(selectedClone,path);
                Global.ExpandCloneInfor("");
            }

            game2D.MouseRequestHandle(false, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            //ReleaseSelectedEntity();
            isMouseDown = false;
            if (isMoveEntity)
            {
                Global.RefreshEditorGrid();
                isMoveEntity = false;
            }
        }


        bool isMoveEntity = false;
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

                MyResult result = game2D.MoveTheSelectedEntity(deltaX, -deltaY);
                if (result.GetResult() == 1)
                {
                    String select = result.GetName();
                    selectedClone = CloneHandle.GetInstance().GetClone(select);
                    selectedClone.X = result.GetX();
                    selectedClone.Y = result.GetY();
                    isMoveEntity = true;
                }
            }
        }

        private void lblRender_MouseDown(object sender, MouseButtonEventArgs e)
        {
            int x = (int)e.GetPosition(lblRender).X;
            int y= (int)e.GetPosition(lblRender).Y;
            txtMouseDownPos.Content = x + "," + y;
            game2D.MouseRequestHandle(true, (int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y, 1);
            isMouseDown = true;
            game2D.ReleaseSelectedEntity();
            MyResult result = game2D.GetPickingEntity((int)e.GetPosition(lblRender).X, (int)e.GetPosition(lblRender).Y);
            if (result.GetResult() == 1)
            {
                String select = result.GetName();
                selectedClone = CloneHandle.GetInstance().GetClone(select);
                Global.ChangePropertyObject(selectedClone);
            }
            else
            {
                Global.ChangePropertyObject(game);
            }
            prevPos = e.GetPosition(lblRender);
            
        }

        private void lblRender_SizeChanged(object sender, SizeChangedEventArgs e)
        {
            Point renderedLocation = lblRender.TranslatePoint(new Point(0, 0), this);
            int x = (int)renderedLocation.X;
            int y = (int)renderedLocation.Y;
            int wid = (int)e.NewSize.Width;
            int hei = (int)e.NewSize.Height;
            try
            {
                game2D.SetWindowSizeChange(x, y, wid, hei);
            }
            catch (Exception ex) { }
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
                game2D.KeyDownRequestHandle(13);
                Global.RefreshEditorGrid();
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
