# win32_Pintu
Windows程序设计拼图游戏
今天有人说，天天拖控件好没意思啊，你能不能不用拖控件写一个小游戏，作为一名C/C++程序员话不多说，直接上才艺
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210316100848475.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NTUyNTI3Mg==,size_16,color_FFFFFF,t_70)

## 这是游戏的效果图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210316100941469.png)
## 搞错了，再来！！！！！

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210316095807784.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NTUyNTI3Mg==,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20210316095830699.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NTUyNTI3Mg==,size_16,color_FFFFFF,t_70)



## 代码构思
先创建一个Windows桌面应用程序拿到一个基本框架

再想 这个拼图游戏要啦些功能（绘制游戏界面，判断胜利，交换位置移动图片......）

首先定义整个游戏要用到的变量

```cpp
#define NEXT 10
//定义宽高
#define WIDTH 130
#define HEIGHT 130
//定义数组
int map[10][10];
//定义设备描述表
HDC memhdc;
//定义鼠标所在行列
int mouse_row, mouse_col;
//定义空白格所在行列 初始是2 2  也就是第九个格子
int row9 = 2, col9 = 2;
```
下面是对游戏的初始化，包括9×9二维数组初始化，加载图片
```cpp

// 初始化
void init()
{
	int s = 0;
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			s++;
			map[i][j] = s;
		}
	}

	//随机交换位置
	int r1, c1, c2, r2;
	int a;
	int b;
	for (int t = 0; t < 100; t++)
	{
		a = rand() % 8 + 1;
		b = rand() % 8 + 1;
		r1 = (a - 1) / 3;
		c1 = (a - 1) % 3;
		r2 = (b - 1) / 3;
		c2 = (b - 1) % 3;
		int m;
		m = map[r1][c1];
		map[r1][c1] = map[r2][c2];
		map[r2][c2] = m;
	}
	// 加载图片
	char buf[100];
	wsprintf(buf, "3.bmp", 1);
	load(buf);
}
```
加载图片的函数
```cpp
void load(char *filename)
{
	HDC hdc = GetDC(hWnd);
	memhdc = ::CreateCompatibleDC(hdc);
	ReleaseDC(hWnd, hdc);
	HBITMAP bmp = (HBITMAP)LoadImage(NULL, filename, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	SelectObject(memhdc, bmp);
}
```
之后就写绘制功能了
```cpp
//绘制游戏界面
void draw()
{
	HDC hdc = GetDC(hWnd);
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] != 9)
			{
				int r, c;
				r = (map[i][j] - 1) / 3;
				c = (map[i][j] - 1) % 3;

				BitBlt(hdc, j*WIDTH, i*HEIGHT, WIDTH, HEIGHT, memhdc, c*WIDTH, r*HEIGHT, SRCCOPY);
			}
			else
			{
				Rectangle(hdc, j*WIDTH, i*HEIGHT, (j + 1)*WIDTH, (i + 1)*HEIGHT);
			}
		}
	ReleaseDC(hWnd, hdc);

}
```
封装好draw函数后，还需要在主界面的回调函数中 PAINT 消息中进行调用

```cpp
	case WM_PAINT:
	{
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: 在此处添加使用 hdc 的任何绘图代码...
		draw();
		EndPaint(hWnd, &ps);
	}
	break;
```
进行上述步骤之后，基本的图像就能绘制出来了

下面是鼠标左键点击的功能编写
```cpp
	// 当鼠标左键按下时候，判断是否能移动，移动后是否拼图完成
	case WM_LBUTTONDOWN:
	{
		// 将鼠标参数传入进行转化
		TurnToRowCol(LOWORD(lParam), HIWORD(lParam));
		if (isNearBlank())	// 判断是否能移动（是否挨着空白格）
		{
			//交换鼠标点击位置与空白位置
			ExChange();
			draw();
			if (isGameOver())	// 判断是否游戏结束
			{
				//显示游戏结束
				if (MessageBox(hWnd, "拼图完成,是否重新开始？", "", MB_OKCANCEL | MB_ICONINFORMATION) == IDOK)
				{
					init();	//重新初始化数据
					draw();	//重新绘制
				}
				else
				{
					DestroyWindow(hWnd);//关闭窗口
				}

			}
		}

	}
	break;
```

交换数组数据功能封装
```cpp
//交换数据
void ExChange()
{
	int tmp = map[mouse_row][mouse_col];
	map[mouse_row][mouse_col] = map[row9][col9];
	map[row9][col9] = tmp;
	row9 = mouse_row;
	col9 = mouse_col;
}
```

当然点击图片时还要判断一下是不是临近空白格，挨着空白格才能交换当前点击的图片与空白的图片位置
```cpp

//判断是否和空白块相邻
int isNearBlank()
{
	if (mouse_row == row9)
	{
		if (abs(col9 - mouse_col) == 1)
		{
			return true;
		}
	}
	if (mouse_col == col9)
	{
		if (abs(mouse_row - row9) == 1)
		{
			return true;
		}
	}
	return false;
}

```
当然每交换完一次还需要进行判断是否胜利
```cpp
//判断游戏结束
int isGameOver()
{
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
		{
			if (map[i][j] != (i * 3 + j + 1))
			{
				return false;
			}
		}
	return true;
}

```
最后在主窗口的回调函数中还加了一个点击鼠标右键直接绘图完成的功能

```cpp
	// 当鼠标右键按下时，直接拼图完成
	case WM_RBUTTONDOWN:
	{
		for (int i = 0; i < 3; i++)
			for (int j = 0; j < 3; j++)
			{
				map[i][j] = i * 3 + j + 1;
			}
		col9 = mouse_col;
		row9 = mouse_row;
		draw();
	}
	break;
```

## OK，大功告成
![在这里插入图片描述](https://img-blog.csdnimg.cn/20210316100743219.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80NTUyNTI3Mg==,size_16,color_FFFFFF,t_70)
## 项目链接
CSDN也设置成零积分了，喜欢的小伙伴可以多多下载，点击左侧上方加号加关注点亮小红心

CSDN：[https://mp.csdn.net/console/upDetailed](https://mp.csdn.net/console/upDetailed)
GitHub：[https://github.com/yang-yongli/win32_Pintu](https://github.com/yang-yongli/win32_Pintu)
