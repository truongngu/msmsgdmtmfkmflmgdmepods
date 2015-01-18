using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Xml;
using System.ComponentModel;

namespace SpriteEditor
{
    public class GameConfig
    {
        public GameConfig()
        {
            _objects.Add("NumOfEntity", new DistributeObject());
        }

        Dictionary<String, DistributeObject> _objects = new Dictionary<string, DistributeObject>();

        string _name;

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
        float _width;

        public float Width
        {
            get { return _width; }
            set { _width = value; }
        }
        float _height;

        public float Height
        {
            get { return _height; }
            set { _height = value; }
        }

        public int NumOfEntity
        {
            get { return int.Parse(_objects["NumOfEntity"].GetAttribute("Value").Value.ToString()); }
            set { _objects["NumOfEntity"].GetAttribute("Value").Value = value; }
        }

        public object this[string propertyName]
        {
            get { return _objects[propertyName].Attributes["Value"].Value; }
            set { _objects[propertyName].SetAttribue("Value", new MyAttribute("Value", value)); }
        }

        Vector2 _cameraPosition;

        public Vector2 CameraPosition
        {
            get { return _cameraPosition; }
            set { _cameraPosition = value; }
        }
    }
}
