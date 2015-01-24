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
    public class GameConfig
    {
        public GameConfig()
        {
            _objects.Add("NumOfEntity", new DistributeObject());
        }

        Dictionary<String, DistributeObject> _objects = new Dictionary<string, DistributeObject>();
        string _name;
        List<EntityInfor> _entities = new List<EntityInfor>();
        MyCollection<EntityInfor> _entityCollection = new MyCollection<EntityInfor>();

        Vector2 _cameraPosition;

        public string Name
        {
            get { return _name; }
            set { _name = value; }
        }

        public MyCollection<EntityInfor> EntityCollection
        {
            get { return _entityCollection; }
            set { _entityCollection = value; }
        }

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
            EntityCollection = new MyCollection<EntityInfor>();

            foreach (XmlNode node in nodeList)
            {
                EntityInfor infor = new EntityInfor();
                infor.LoadFromXml(node);

                Entities.Add(infor);
                EntityCollection.Add(infor);
            }
        }

        public void WriteToXml(string path)
        {
            XmlDocument doc = new XmlDocument();
            //doc.Load(path);
            if (doc != null)
            {
                XmlElement root = doc.CreateElement("CONFIG");
                root.SetAttribute("name", Name);
                doc.AppendChild(root);
                XmlNode entityList = doc.CreateElement("ENTITY_LIST");
                root.AppendChild(entityList);
                for (int t = 0; t < EntityCollection.Count; t++)
                {
                    EntityInfor infor = EntityCollection[t];

                    XmlElement entity = doc.CreateElement("ENTITY");
                    XmlElement TEXTURE_PATH = doc.CreateElement("TEXTURE_PATH");
                    XmlElement CLASS_NAME = doc.CreateElement("CLASS_NAME");
                    XmlElement SPRITE_ATTRIBUTE = doc.CreateElement("SPRITE_ATTRIBUTE");
                    XmlElement SHADER = doc.CreateElement("SHADER");
                    XmlElement FRAME_INFO = doc.CreateElement("FRAME_INFO");
                    XmlElement CLONE_INFO = doc.CreateElement("CLONE_INFO");

                    TEXTURE_PATH.InnerText = infor.TexturePath;
                    CLASS_NAME.InnerText = infor.ClassName;
                    SPRITE_ATTRIBUTE.SetAttribute("fps", infor.Fps.ToString());
                    SPRITE_ATTRIBUTE.SetAttribute("width", infor.Width.ToString());
                    SPRITE_ATTRIBUTE.SetAttribute("height", infor.Height.ToString());
                    SPRITE_ATTRIBUTE.SetAttribute("num_spriteX", infor.NumSpriteX.ToString());
                    SPRITE_ATTRIBUTE.SetAttribute("num_spriteY", infor.NumSpriteY.ToString());
                    SPRITE_ATTRIBUTE.SetAttribute("total_sprite", infor.TotalSprite.ToString());

                    SHADER.SetAttribute("vertex", infor.VertexShader.ToString());
                    SHADER.SetAttribute("fragment", infor.FragmentShader.ToString());

                    FRAME_INFO.SetAttribute("num_frame", infor.NumFrame.ToString());
                    int n = infor.NumFrame;
                    for (int i = 0; i < n; i++)
                    {
                        XmlElement frame = doc.CreateElement("FRAME");
                        infor.Frames[i].WriteToXml(frame);
                        FRAME_INFO.AppendChild(frame);
                    }

                    CLONE_INFO.SetAttribute("num_clone", infor.NumClone.ToString());
                    n = infor.NumClone;
                    for (int i = 0; i < n; i++)
                    {
                        XmlElement clone = doc.CreateElement("CLONE");
                        infor.Clones[i].WriteToXml(clone);
                        CLONE_INFO.AppendChild(clone);
                    }


                    entity.AppendChild(TEXTURE_PATH);
                    entity.AppendChild(CLASS_NAME);
                    entity.AppendChild(SPRITE_ATTRIBUTE);
                    entity.AppendChild(SHADER);
                    entity.AppendChild(FRAME_INFO);
                    entity.AppendChild(CLONE_INFO);
                    entityList.AppendChild(entity);
                }

                doc.Save(path);
            }
            
        }

        public void Add(EntityInfor infor, string path2XML)
        {
            Entities.Add(infor);
            EntityCollection.Add(infor);
            XmlDocument doc = new XmlDocument();
            doc.Load(path2XML);
            if (doc != null)
            {
                XmlElement root = doc.DocumentElement;
                XmlNode entityList = root.SelectSingleNode("ENTITY_LIST");
                XmlElement entity = doc.CreateElement("ENTITY");
                XmlElement TEXTURE_PATH = doc.CreateElement("TEXTURE_PATH");
                XmlElement CLASS_NAME = doc.CreateElement("CLASS_NAME");
                XmlElement SPRITE_ATTRIBUTE = doc.CreateElement("SPRITE_ATTRIBUTE");
                XmlElement SHADER = doc.CreateElement("SHADER");
                XmlElement FRAME_INFO = doc.CreateElement("FRAME_INFO");
                XmlElement CLONE_INFO = doc.CreateElement("CLONE_INFO");

                TEXTURE_PATH.InnerText = infor.TexturePath;
                CLASS_NAME.InnerText = infor.ClassName;
                SPRITE_ATTRIBUTE.SetAttribute("fps", infor.Fps.ToString());
                SPRITE_ATTRIBUTE.SetAttribute("width", infor.Width.ToString());
                SPRITE_ATTRIBUTE.SetAttribute("height", infor.Height.ToString());
                SPRITE_ATTRIBUTE.SetAttribute("num_spriteX", infor.NumSpriteX.ToString());
                SPRITE_ATTRIBUTE.SetAttribute("num_spriteY", infor.NumSpriteY.ToString());
                SPRITE_ATTRIBUTE.SetAttribute("total_sprite", infor.TotalSprite.ToString());

                SHADER.SetAttribute("vertex", infor.VertexShader.ToString());
                SHADER.SetAttribute("fragment", infor.FragmentShader.ToString());

                FRAME_INFO.SetAttribute("num_frame", infor.NumFrame.ToString());
                int n = infor.NumFrame;
                for (int i = 0; i < n; i++)
                {
                    XmlElement frame = doc.CreateElement("FRAME");
                    frame.SetAttribute("name", infor.Frames[i].Name);
                    List<int> content = infor.Frames[i].FrameContent;
                    int m = content.Count;
                    String frameContent = "";
                    for (int j = 0; j < m; j++)
                    {
                        frameContent += content[j] + " ";
                    }
                    frame.InnerText = frameContent;
                    FRAME_INFO.AppendChild(frame);
                }

                CLONE_INFO.SetAttribute("num_clone", infor.NumClone.ToString());
                n = infor.NumClone;
                for (int i = 0; i < n; i++)
                {
                    XmlElement clone = doc.CreateElement("CLONE");
                    clone.SetAttribute("name", infor.Clones[i].Name);
                    clone.SetAttribute("x", infor.Clones[i].X.ToString());
                    clone.SetAttribute("y", infor.Clones[i].Y.ToString());
                    clone.SetAttribute("z", infor.Clones[i].Z.ToString());
                    clone.SetAttribute("usedata", infor.Clones[i].UserData.ToString());
                    CLONE_INFO.AppendChild(clone);
                }


                entity.AppendChild(TEXTURE_PATH);
                entity.AppendChild(CLASS_NAME);
                entity.AppendChild(SPRITE_ATTRIBUTE);
                entity.AppendChild(SHADER);
                entity.AppendChild(FRAME_INFO);
                entity.AppendChild(CLONE_INFO);
                entityList.AppendChild(entity);
            }
            doc.Save(path2XML);
        }
    }
}
