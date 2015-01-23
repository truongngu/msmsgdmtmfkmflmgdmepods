using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using System.Xml;
using System.ComponentModel;
using System.Collections;

namespace SpriteEditor
{
    public class GameConfig : CollectionBase, ICustomTypeDescriptor
    {
        public GameConfig()
        {
            _objects.Add("NumOfEntity", new DistributeObject());
        }

        Dictionary<String, DistributeObject> _objects = new Dictionary<string, DistributeObject>();
        string _name;
        List<EntityInfor> _entities = new List<EntityInfor>();
        Vector2 _cameraPosition;

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }
        
        [TypeConverter(typeof(ExpandableObjectConverter))]
        public List<EntityInfor> Entities
        {
            get { return _entities; }
            set { _entities = value; }
        }

        public Vector2 CameraPosition
        {
            get { return _cameraPosition; }
            set { _cameraPosition = value; }
        }

        public void LoadFromXml(string path)
        {
            XmlDocument doc = new XmlDocument();
            doc.Load(path);

            XmlElement root = doc.DocumentElement;

            Name = root.Attributes["name"].Value;

            XmlNodeList nodeList = root.SelectNodes("ENTITY_LIST/ENTITY");

            Entities = new List<EntityInfor>();

            foreach (XmlNode node in nodeList)
            {
                EntityInfor infor = new EntityInfor();
                infor.LoadFromXml(node);

                Entities.Add(infor);
            }
        }

        public void WriteToXml(string path)
        {
            
        }

        public void Add(EntityInfor infor)
        {

        }
    }
}
