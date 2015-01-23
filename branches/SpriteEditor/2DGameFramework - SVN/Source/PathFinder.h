#pragma once
#include <vector>
#include "DebugDefine.h"
#include "..\Utilities\utilities.h"
using namespace std;


enum Dir
{
	Bac,
	Dong,
	Nam,
	Tay,
	DongBac,
	DongNam,
	TayNam,
	TayBac,
	Khong,
};
enum MoveStyle
{
	FourCell,
	EightCell
};
struct Node
{
public:
	Vector2 pos;//vi tri thuc
    int parentIndex;//luu vet
    int G;//trong so 2 o ke
    int h;//heritic
    int f;//tong chi phi
	Node(Vector2 p, int parent, int tg, int th, int tf)
    {
        pos = p;
        parentIndex = parent;
        G = tg;
        h = th;
        f = tf;
    }
};

class PathFinder
{
private:
public:
	static float xmin,xmax,ymin,ymax;
	static MoveStyle moveStyle;	
	Node* startNode;
	Node* endNode;
	PathFinder(void);
	~PathFinder(void);

	vector<Vector2> findPath(Vector2 startPos,Vector2 endPos);
	int cost(Node* node1, Node*node2);
	int heuristic(Node* node1,Node* node2);

	static vector<Vector2> getAllAdjCells(Vector2 pos, MoveStyle moveStyle);
	static Vector2 getAdjCellFromDir(Dir dir, Vector2 in);
	static Dir getDirVector2(Vector2 root, Vector2 adj);
};


