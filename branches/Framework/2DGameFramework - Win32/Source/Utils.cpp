#include "Utils.h"
#include "Global.h"
#include "DebugDefine.h"
#include <time.h>
#ifdef Android
#include <unistd.h>
#endif

void ReadLine(FILE *pFile, char* buffer)
{
	int i = 0;
	char c;
	while(true)
	{
		if (feof(pFile))
		{
			buffer[i-1] = '\0';
			return;
		}
		fread(&c, 1, 1, pFile);
		if (c == '\n')
		{
			buffer[i] = '\0';
			break;
		}
		buffer[i++] = c;
	}
}

bool ReadToKey(FILE* pf, char key)
{
	char c;
	while (true)
	{
		if (feof(pf))
			return false;
		fread(&c, 1, 1, pf);
		if (c == key)
		{
			if (key == '/')
			{
				fread(&c, 1, 1, pf);
				if (c == key)
					return true;
				return false;
			}
			else
				return true;
		}
	}
}

int SeekToNumber(char* buffer, int iStart)
{
	while(true)
	{
		if (buffer[iStart] == '-' && buffer[iStart + 1] >= '0' && buffer[iStart + 1] <= '9')
			return iStart;

		if (buffer[iStart] >= '0' && buffer[iStart] <= '9')
			return iStart;

		// End buffer
		if (buffer[iStart] == '\n')
			return -1;
		++iStart;
	}
}

int ScanNumberInt32(char* buffer, int *iNumber, int iStart)
{
	iStart = SeekToNumber(buffer, iStart);
	if (iStart == -1)
		return -1;

	int iSign = 1;
	if (buffer[iStart] == '-')
	{
		iSign = -1;
		++iStart;
	}

	*iNumber = 0;
	while(buffer[iStart] >= '0' && buffer[iStart] <= '9')
	{
		*iNumber = (*iNumber)*10 + buffer[iStart++] - '0';
	}

	*iNumber *= iSign;

	return iStart;
}

int ScanNumberUInt32(char* buffer, unsigned int *iNumber, int iStart)
{
	int iTmp;
	int iPos = ScanNumberInt32(buffer, &iTmp, iStart);
	*iNumber = iTmp;

	return iPos;
}

int ScanNumberFloat32(char* buffer, float *iNumber, int iStart)
{
	iStart = SeekToNumber(buffer, iStart);
	if (iStart == -1)
		return -1;

	int iSign = 1;
	if (buffer[iStart] == '-')
	{
		iSign = -1;
		++iStart;
	}

	*iNumber = 0.0f;
	while(buffer[iStart] >= '0' && buffer[iStart] <= '9')
	{
		*iNumber = (*iNumber)*10 + buffer[iStart++] - '0';
	}

	if (buffer[iStart] != '.')
		return iStart;

	float fTmp2 = 1.0f;
	++iStart;

	while(buffer[iStart] >= '0' && buffer[iStart] <= '9')
	{
		fTmp2 /= 10.0f;
		*iNumber += (buffer[iStart] - '0')*fTmp2;
		++iStart;
	}

	*iNumber *= iSign;

	return iStart;
}

int SeekToQuote(char* buffer, int iStart)
{
	while(true)
	{
		if (buffer[iStart] == '\"' )
			return iStart;

		// End buffer
		if (buffer[iStart] == '\n')
			return -1;
		++iStart;
	}
}

int ScanPath(char* buffer, char* Path, int iStart)
{
	iStart = SeekToQuote(buffer, iStart);
	if (iStart == -1)
		return -1;
	iStart++;
	int i = 0;
	while(buffer[iStart] != '\n' && buffer[iStart] != '\"')
	{
		Path[i] = buffer[iStart];
		iStart++;
		i++;
	}
	Path[i] = '\0';
	return iStart;
}


bool IsInCameraView(Entity2D* obj)
{
	return true;
}

unsigned int Randx=RANDOMSEEDX;
unsigned int Randy=RANDOMSEEDY;
unsigned int Randz=RANDOMSEEDZ; 
unsigned int Randw;
unsigned int GetARandomNumber(void) {
    unsigned int t = Randx ^ (Randx << 11);
    Randx = Randy; Randy = Randz; Randz = Randw;
    return Randw = Randw ^ (Randw >> 19) ^ t ^ (t >> 8);
}



Matrix InvertMatrix( Matrix M)
{
	Matrix out;
	long indxc[4], indxr[4], ipiv[4];
	long i, icol, irow, j, ir, ic;
	float big, dum, pivinv, temp, bb;
	ipiv[0] = -1;
	ipiv[1] = -1;
	ipiv[2] = -1;
	ipiv[3] = -1;
	 out.m[0][0] = M.m[0][0];
	 out.m[1][0] = M.m[1][0];
	 out.m[2][0] = M.m[2][0];
	 out.m[3][0] = M.m[3][0];
	 out.m[0][1] = M.m[0][1];
	 out.m[1][1] = M.m[1][1];
	 out.m[2][1] = M.m[2][1];
	 out.m[3][1] = M.m[3][1];
	 out.m[0][2] = M.m[0][2];
	 out.m[1][2] = M.m[1][2];
	 out.m[2][2] = M.m[2][2];
	 out.m[3][2] = M.m[3][2];
	 out.m[0][3] = M.m[0][3];
	 out.m[1][3] = M.m[1][3];
	 out.m[2][3] = M.m[2][3];
	 out.m[3][3] = M.m[3][3];
	 for (i = 0; i < 4; i++) {
		 big = 0.0f;
		for (j = 0; j < 4; j++) {
		if (ipiv[j] != 0) {
			if (ipiv[0] == -1) {
				if ((bb = (float) Abs(out.m[j][0])) > big) {
				 big = bb;
				 irow = j;
				 icol = 0;
			}
			} else if (ipiv[0] > 0) {
				return Matrix().SetZero();
		}
		if (ipiv[1] == -1) {
			if ((bb = (float) Abs((float) out.m[j][1])) > big) {
			 big = bb;
			 irow = j;
			 icol = 1;
		 }
		} else if (ipiv[1] > 0) {
			return Matrix().SetZero();
		}
		if (ipiv[2] == -1) {
		 if ((bb = (float) Abs((float) out.m[j][2])) > big)  {
			 big = bb;
			 irow = j;
			 icol = 2;
		 }
		} else if (ipiv[2] > 0) {
			return Matrix().SetZero();
		}
	 if (ipiv[3] == -1) {
		if ((bb = (float) Abs((float) out.m[j][3])) > big) {
			 big = bb;
			 irow = j;
			 icol = 3;
		 }
	 } else if (ipiv[3] > 0) {
		return Matrix().SetZero();
	 }
	}
   }
	++(ipiv[icol]);
	if (irow != icol) {
		 temp = out.m[irow][0];
		 out.m[irow][0] = out.m[icol][0];
		 out.m[icol][0] = temp;
		 temp = out.m[irow][1];
		 out.m[irow][1] = out.m[icol][1];
		 out.m[icol][1] = temp;
		 temp = out.m[irow][2];
		 out.m[irow][2] = out.m[icol][2];
		 out.m[icol][2] = temp;
		 temp = out.m[irow][3];
		 out.m[irow][3] = out.m[icol][3];
		 out.m[icol][3] = temp;
	}
	 indxr[i] = irow;
	 indxc[i] = icol;
	if (out.m[icol][icol] == 0.0) {
		return out;
	}
	 pivinv = 1.0f / out.m[icol][icol];
	 out.m[icol][icol] = 1.0f;
	 out.m[icol][0] *= pivinv;
	 out.m[icol][1] *= pivinv;
	 out.m[icol][2] *= pivinv;
	 out.m[icol][3] *= pivinv;
	 if (icol != 0) {
		 dum = out.m[0][icol];
		 out.m[0][icol] = 0.0f;
		 out.m[0][0] -= out.m[icol][0] * dum;
		 out.m[0][1] -= out.m[icol][1] * dum;
		 out.m[0][2] -= out.m[icol][2] * dum;
		 out.m[0][3] -= out.m[icol][3] * dum;
	 }
	if (icol != 1) {
		 dum = out.m[1][icol];
		 out.m[1][icol] = 0.0f;
		 out.m[1][0] -= out.m[icol][0] * dum;
		 out.m[1][1] -= out.m[icol][1] * dum;
		 out.m[1][2] -= out.m[icol][2] * dum;
		 out.m[1][3] -= out.m[icol][3] * dum;
	}
	if (icol != 2) {
		 dum = out.m[2][icol];
		 out.m[2][icol] = 0.0f;
		 out.m[2][0] -= out.m[icol][0] * dum;
		 out.m[2][1] -= out.m[icol][1] * dum;
		 out.m[2][2] -= out.m[icol][2] * dum;
		 out.m[2][3] -= out.m[icol][3] * dum;
	}
	if (icol != 3) {
		 dum = out.m[3][icol];
		 out.m[3][icol] = 0.0f;
		 out.m[3][0] -= out.m[icol][0] * dum;
		 out.m[3][1] -= out.m[icol][1] * dum;
		 out.m[3][2] -= out.m[icol][2] * dum;
		 out.m[3][3] -= out.m[icol][3] * dum;
	}
  }
	 if (indxr[3] != indxc[3]) {
	 ir = indxr[3];
	 ic = indxc[3];
	 temp = out.m[0][ir];
	 out.m[0][ir] = out.m[0][ic];
	 out.m[0][ic] = temp;
	 temp = out.m[1][ir];
	 out.m[1][ir] = out.m[1][ic];
	 out.m[1][ic] = temp;
	 temp = out.m[2][ir];
	 out.m[2][ir] = out.m[2][ic];
	 out.m[2][ic] = temp;
	 temp = out.m[3][ir];
	 out.m[3][ir] = out.m[3][ic];
	 out.m[3][ic] = temp;
	 }
	 if (indxr[2] != indxc[2]) {
	 ir = indxr[2];
	 ic = indxc[2];
	 temp = out.m[0][ir];
	 out.m[0][ir] = out.m[0][ic];
	 out.m[0][ic] = temp;
	 temp = out.m[1][ir];
	 out.m[1][ir] = out.m[1][ic];
	 out.m[1][ic] = temp;
	 temp = out.m[2][ir];
	 out.m[2][ir] = out.m[2][ic];
	 out.m[2][ic] = temp;
	 temp = out.m[3][ir];
	 out.m[3][ir] = out.m[3][ic];
	 out.m[3][ic] = temp;
	 }
	 if (indxr[1] != indxc[1]) {
	 ir = indxr[1];
	 ic = indxc[1];
	 temp = out.m[0][ir];
	 out.m[0][ir] = out.m[0][ic];
	 out.m[0][ic] = temp;
	 temp = out.m[1][ir];
	 out.m[1][ir] = out.m[1][ic];
	 out.m[1][ic] = temp;
	 temp = out.m[2][ir];
	 out.m[2][ir] = out.m[2][ic];
	 out.m[2][ic] = temp;
	 temp = out.m[3][ir];
	 out.m[3][ir] = out.m[3][ic];
	 out.m[3][ic] = temp;
	 }
	 if (indxr[0] != indxc[0]) {
	 ir = indxr[0];
	 ic = indxc[0];
	 temp = out.m[0][ir];
	 out.m[0][ir] = out.m[0][ic];
	 out.m[0][ic] = temp;
	 temp = out.m[1][ir];
	 out.m[1][ir] = out.m[1][ic];
	 out.m[1][ic] = temp;
	 temp = out.m[2][ir];
	 out.m[2][ir] = out.m[2][ic];
	 out.m[2][ic] = temp;
	 temp = out.m[3][ir];
	 out.m[3][ir] = out.m[3][ic];
	 out.m[3][ic] = temp;
	 }
	 return out;
}

double Det(Matrix m,int n)
{
	return -1;
}

double Distance(Vector3 pos1,Vector3 pos2)
{
	return sqrt((pos1.x-pos2.x)*(pos1.x-pos2.x)+(pos1.y-pos2.y)*(pos1.y-pos2.y)+(pos1.z-pos2.z)*(pos1.z-pos2.z));
}

float Abs(float value)
{
	if (value>=0) return value;
	return -value;
}

int ArrCharLen(char* arr)
{
	if (arr==0)
		return 0;
	int i=0;
	while(arr[i]!='\0')
		i++;
	return i;
}

#if defined WindowPhone
Platform::String^ StringFromAscIIChars(char* chars)
{
	size_t newsize = strlen(chars) + 1;
	wchar_t * wcstring = new wchar_t[newsize];
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, wcstring, newsize, chars, _TRUNCATE);
	Platform::String^ str = ref new Platform::String(wcstring);
	delete[] wcstring;
	return str;
}
#endif

void Log(const char* s)
{
#ifdef Win32
	esLogMessage(s);
	esLogMessage("\n");
	printf(s);
	printf("\n");
#endif
#ifdef Android
	LOGE(s);
#endif
#ifdef WindowPhone
	OutputDebugString(s);
#endif
}

float Roundf(float value)
{
	int res;
	if (value >= floor(value)+0.5) res = ceil(value);
	else res = floor(value);
	return res;
}

Vector3 ConvertCoordinate2D3D(BaseCamera* cam,Vector2 pos2D)
{
	double x = 2.0 * pos2D.x / Global::ScreenWidth - 1;
	double y = - 2.0 * pos2D.y / Global::ScreenHeight + 1;
	Matrix vp=cam->ViewMatrix()*cam->projection;
	Matrix invert=InvertMatrix(vp);
	Vector4 pos=Vector4(x,y,0,0);

	pos=pos*invert;
	Vector3 camPos=cam->GetPosition();
	if (cam->IsOrthorView())
		return Vector3(pos.x+camPos.x,pos.y+camPos.y,0.0f);
	else{
		pos.x=pos.x*(camPos.z)+camPos.x;///ad->GetPosition().z);
		pos.y=pos.y*(camPos.z)+camPos.y;///ad->GetPosition().z);
		return Vector3(pos.x,pos.y,0.0f);
	}
}

Vector3 ConvertCoordinate2D3DByZIndex(BaseCamera* cam, Vector2 pos2D, float zindex)
{
	double x = 2.0 * pos2D.x / Global::ScreenWidth - 1;
	double y = -2.0 * pos2D.y / Global::ScreenHeight + 1;
	Matrix vp = cam->ViewMatrix()*cam->projection;
	Matrix invert = InvertMatrix(vp);
	Vector4 pos = Vector4(x, y, 0, 0);

	pos = pos*invert;
	Vector3 camPos = cam->GetPosition();
	if (cam->IsOrthorView())
		return Vector3(pos.x + camPos.x, pos.y + camPos.y, 0.0f);
	else{
		pos.x = pos.x*(camPos.z-zindex) + camPos.x;///ad->GetPosition().z);
		pos.y = pos.y*(camPos.z-zindex) + camPos.y;///ad->GetPosition().z);
		return Vector3(pos.x, pos.y, 0.0f);
	}
}

Vector2 ConvertCoordinate3D2D(BaseCamera* cam,Vector3 pos3D)
{
	 Matrix vp = cam->projection * cam->ViewMatrix();
	 Vector4 pos=Vector4(pos3D);
      //transform world to clipping coordinates
     pos = pos*vp;

      int winX = (int) Roundf((( pos.x + 1 ) / 2.0) *
		  Global::ScreenWidth );
      int winY = (int) Roundf((( 1 - pos.y ) / 2.0) *
		  Global::ScreenHeight );
      return Vector2(winX,winY);
}


float MaxF(float v1,float v2)
{
	return v1>=v2? v1:v2;
}

float MinF(float v1,float v2)
{
	return v1<=v2? v1:v2;
}


float GetTimeNow()
{
#ifdef Win32
	return clock();
#endif
#ifdef Android
	struct timespec res;
	clock_gettime(CLOCK_REALTIME, &res);
	return 1000.0 * res.tv_sec + (double)res.tv_nsec / 1e6;
#endif
#ifdef WindowPhone
	return clock();
#endif
}
#ifdef WindowPhone
struct WaitTimeData
{
	CONDITION_VARIABLE conditionVariable;
	CRITICAL_SECTION criticalSection;

	WaitTimeData()
	{
		InitializeConditionVariable(&conditionVariable);
		memset(&criticalSection, 0, sizeof(CRITICAL_SECTION));
		//InitializeCriticalSection(&_CriticalSection); wp8 has no this func
		criticalSection.DebugInfo = (PRTL_CRITICAL_SECTION_DEBUG)-1;
		criticalSection.LockCount = -1;
	}
};

int WPSleep(unsigned int ms)
{
	static WaitTimeData s_wtd;

	SleepConditionVariableCS(
		&s_wtd.conditionVariable,
		&s_wtd.criticalSection,
		ms
		);

	if (GetLastError() == ERROR_TIMEOUT)
	{
		SetLastError(0);
	}

	return 0;
}
#endif
void SleepForATime(float ms)
{
#ifdef Win32
	Sleep(ms);
#endif
#ifdef WindowPhone
	WPSleep(ms);
#endif
#ifdef Anroid
	sleep(ms);
#endif

}

bool checkIfExistInResourcesDictory(string path2File){
	string path = Global::gameResourceDir + path2File;
	FILE* file;
	file = fopen(path.c_str(), "r");
	if (file == 0){
		return false;
	}
	fclose(file);
	return true;
}
