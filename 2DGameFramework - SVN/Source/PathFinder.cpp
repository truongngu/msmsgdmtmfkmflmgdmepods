#include "PathFinder.h"
#include "Global.h"
#include <math.h>
#include <list>
#include "Utils.h"

float PathFinder::xmax=0;
float PathFinder::xmin=0;
float PathFinder::ymin=0;
float PathFinder::ymax=0;
MoveStyle PathFinder::moveStyle=FourCell;

Vector2 PathFinder::getAdjCellFromDir(Dir dir, Vector2 in)
{
	int i = in.y;
	int j = in.x;
	switch (dir)
	{
	case Nam:
	{
				i = i - 1;
				j = j;
	}
		break;
	case TayBac:
	{
				   i = i - 1;
				   j = j - 1;
	}
		break;
	case Tay:
	{
				i = i;
				j = j - 1;
	}
		break;
	case TayNam:
	{
				   i = i + 1;
				   j = j - 1;
	}
		break;
	case Bac:
	{
				i = i + 1;
				j = j;
	}
		break;
	case DongNam:
	{
					i = i + 1;
					j = j + 1;
	}
		break;
	case Dong:
	{
				 i = i;
				 j = j + 1;
	}
		break;
	case DongBac:
	{
					i = i - 1;
					j = j + 1;
	}
		break;
	default:
		break;
	}
	/*int maxRows = Globals::terrain->MapRows/2;
	int maxCols = Globals::terrain->MapColumns/2;*/
	if (i<=ymax && j<=xmax&&i>=ymin&&j>=xmin)
		return Vector2(j, i);
	else
		return Vector2(MAXINT, MAXINT);
}

vector<Vector2> PathFinder::getAllAdjCells(Vector2 pos,MoveStyle m)
{
	vector<Vector2> out;
	if (m==MoveStyle::EightCell)
	{
		for (int i = 0; i < 8; i++)
		{
			out.push_back(getAdjCellFromDir((Dir)i, pos));
		}
	}
	else if (m==MoveStyle::FourCell)
	{
		int beginDir=GetARandomNumber()%4;
		switch(beginDir)
		{
		case 0:
			for (int i = 0; i < 4; i++)
			{
				out.push_back(getAdjCellFromDir((Dir)i, pos));
			}
			break;
		case 1:
			out.push_back(getAdjCellFromDir((Dir)1, pos));
			out.push_back(getAdjCellFromDir((Dir)2, pos));
			out.push_back(getAdjCellFromDir((Dir)3, pos));
			out.push_back(getAdjCellFromDir((Dir)0, pos));
			break;
		case 2:
			out.push_back(getAdjCellFromDir((Dir)2, pos));
			out.push_back(getAdjCellFromDir((Dir)3, pos));
			out.push_back(getAdjCellFromDir((Dir)0, pos));
			out.push_back(getAdjCellFromDir((Dir)1, pos));
			break;
		case 3:
			out.push_back(getAdjCellFromDir((Dir)3, pos));
			out.push_back(getAdjCellFromDir((Dir)0, pos));
			out.push_back(getAdjCellFromDir((Dir)1, pos));
			out.push_back(getAdjCellFromDir((Dir)2, pos));
			break;
		}
		
	}
	return out;
}


bool isEqualVector2(Vector2 vec1, Vector2 vec2)
{
	if (vec1.x == vec2.x&&vec1.y == vec2.y)
		return true;
	return false;
}




PathFinder::PathFinder(void)
{
	startNode =  new Node(Vector2(), -1, -1, -1, -1);
    endNode = new Node(Vector2(), -1, -1, -1, -1);
}


PathFinder::~PathFinder(void)
{
	/*Delete(startNode);*/
	Delete(endNode);
}

vector<Vector2> PathFinder::findPath(Vector2 startPos,Vector2 endPos)
{
	Delete(startNode);
	Delete(endNode);
	startNode =  new Node(startPos, -1, -1, -1, -1);
    endNode = new Node(endPos, -1, -1, -1, -1);
	vector<Node*> open;
	vector<Node*> closed;
	vector<Node*> groupNode;
	int G = 0;
    int h = heuristic(startNode, endNode);
    int f = G + h;

	open.push_back(startNode);
	while (open.size() > 0)
    {
		//Node* best=open.at(open.size()-1);
		int bestCost = open.at(0)->f;
		int bestNode = 0;
		int nt = open.size();
		for (int i = 0; i < nt; i++)
		{
		    if (open.at(i)->f < bestCost)
		    {
		        bestCost = open.at(i)->f;
		        bestNode = i;
		    }
		}
		/*open.pop_back();*/
		Node* currentNode=open.at(bestNode);
		//Da tim thay
		if (currentNode->pos.x == endNode->pos.x && currentNode->pos.y == endNode->pos.y)
		{
			vector<Vector2> res;
			res.push_back(endNode->pos);
			
			while (currentNode->parentIndex != -1)
            {
				currentNode = closed.at(currentNode->parentIndex);
                //currentNode.mapCell = map.World2MapCell(currentNode.pos);
				/*vector<Vector2>::iterator it=res.begin();
				res=res.insert(it,currentNode->pos);*/
				res.push_back(currentNode->pos);
            }
			int n = closed.size();
			for (int i = 0; i < n; i++)
			{
				Delete(closed[i]);
			}
			n = open.size();
			for (int i = 0; i < n; i++)
			{
				Delete(open[i]);
			}
			return res;
           // return path;
		}
		else
		{
			open.erase(open.begin() + bestNode);
			groupNode.push_back(currentNode);
			closed.push_back(currentNode);
			if (closed.size() < 120)
            {
				vector<Vector2> adjCells = getAllAdjCells(currentNode->pos,moveStyle);
				adjCells.push_back(currentNode->pos);
				for (int i = 0; i < adjCells.size(); i++)
				{
					Vector2 pos = adjCells.at(i);

					if (!isEqualVector2(pos,Vector2(MAXINT,MAXINT)) && (true || isEqualVector2(endNode->pos,pos)))
					{
						bool found_in_closed = false;
						int n = closed.size();
						for (int i = 0; i < n; i++)
						{
							Node* temp = closed.at(i);
							if (isEqualVector2(temp->pos, pos))
							{
								found_in_closed = true;
								break;
							}
						}
						if (found_in_closed)
							continue;

						//see if the Node is in our open list. If not, use it.
						bool found_in_open = false;
						//foreach (var Node2 in open)
						n = open.size();
						for (int num = 0; num < n; num++)
						{
							Node* temp = open.at(num);
							if (isEqualVector2(temp->pos,pos))
							{
								found_in_open = true;
								break;
							}
						}

						if (!found_in_open)
						{
							Node* newNode = new Node(pos, closed.size() - 1, -1, -1, -1);

							newNode->G = currentNode->G + cost(currentNode, newNode);
							newNode->h = heuristic(newNode, endNode);
							newNode->f = newNode->G + newNode->h;
							open.push_back(newNode);
						}
					}
				}
			}
			else
			{
				vector<Vector2> res;
				res.push_back(endNode->pos);
				while (currentNode->parentIndex != -1)
				{
					currentNode = closed.at(currentNode->parentIndex);
					//currentNode.mapCell = map.World2MapCell(currentNode.pos);
					/*vector<Vector2>::iterator it=res.begin();
					res=res.insert(it,currentNode->pos);*/
					res.push_back(currentNode->pos);
				}
				int n = closed.size();
				for (int i = 0; i < n; i++)
				{
					Delete(closed[i]);
				}
				n = open.size();
				for (int i = 0; i < n; i++)
				{
					Delete(open[i]);
				}
				return res;
			}
		}
		
	}
	int n = closed.size();
	for (int i = 0; i < n; i++)
	{
		Delete(closed[i]);
	}
	n = open.size();
	for (int i = 0; i < n; i++)
	{
		Delete(open[i]);
	}
	return vector<Vector2>();
}

int PathFinder::heuristic(Node* currentNode, Node* endNode)
{

	float dx = Abs(currentNode->pos.x - endNode->pos.x);
    float dy = Abs(currentNode->pos.y - endNode->pos.y);
    int D = 1;

    float dx1 = currentNode->pos.x - endNode->pos.x;
    float dy1 = currentNode->pos.y - endNode->pos.y;
    float dx2 = startNode->pos.x - endNode->pos.x;
    float dy2 = startNode->pos.y - endNode->pos.y;

    float cross = Abs(dx1 * dy2 - dx2 * dy1) * 4;

    float heuristic = 0;
	heuristic += cross * 0.01f;

    return (int)heuristic;
}



Dir getDir(Node* root, Node *adj)
{
	int i = adj->pos.y - root->pos.y;
	int j = adj->pos.x - root->pos.x;
	if (i == -1 && j == 0)
		return Dir::Nam;
	else if (i == -1 && j == -1)
		return Dir::TayBac;
	else if (i == 0 && j == -1)
		return Dir::Tay;
	else if (i == 1 && j == -1)
		return Dir::TayNam;
	else if (i == 1 && j == 0)
		return Dir::Bac;
	else if (i == 1 && j == 1)
		return Dir::DongNam;
	else if (i == 0 && j == 1)
		return Dir::Dong;
	else if (i == -1 && j == 1)
		return Dir::DongBac;
	return Dir::Khong;
}

int  PathFinder::cost(Node*currentNode, Node*newNode)
{
	int score = 0;
	Dir direction = getDir(currentNode, newNode);
	
	switch (direction)
	{
	case Dong:case Tay:case Bac: case Nam:
		return 24;
	case DongBac:case DongNam: case TayBac: case TayNam:
		return 32;
	default:
		break;
	}
	
	return MAXINT;
}


Dir PathFinder::getDirVector2(Vector2 root, Vector2 adj)
{
	int i = adj.y - root.y;
	int j = adj.x - root.x;
	if (i == -1 && j == 0)
		return Dir::Nam;
	else if (i == -1 && j == -1)
		return Dir::TayBac;
	else if (i == 0 && j == -1)
		return Dir::Tay;
	else if (i == 1 && j == -1)
		return Dir::TayNam;
	else if (i == 1 && j == 0)
		return Dir::Bac;
	else if (i == 1 && j == 1)
		return Dir::DongNam;
	else if (i == 0 && j == 1)
		return Dir::Dong;
	else if (i == -1 && j == 1)
		return Dir::DongBac;
	return Dir::Khong;
}

