#include <vector>
#include <cmath>
#include "raylib.h"
#include "raymath.h"

#define Quant 250

#define ScreenX 800
#define ScreenY 450

using namespace std;

int offsetX = 50;
int offsetY = 10;

const float Gravity = 100;
const float Floor = ScreenY-offsetY;
const float Fricton = 0.5;
const float GridTam = 20;

const int quantGridX = (ScreenX-2*offsetX)/GridTam;
const int quantGridY = (ScreenY-2*offsetY)/GridTam;

class Ball
{
    public:
    int ID;
    Vector2 pos;
    Vector2 vel;
    Vector2 acel;
    Vector2 Grid;
    float radius;
    Color color = RED;
    
    Ball(int _ID,float _x,float _y,float _velx,float _vely,float _radius)
    {
        ID=_ID;
        pos.x=_x;
        pos.y=_y;
        vel.x=_velx;
        vel.y=_vely;
        radius=_radius;
        acel.x=0;
        acel.y=0;
        
    }
};

vector<Ball> Balls;
//Ball *GridList[20][40];

void CreateBalls(int quant, int createMouse)
{
    for(int i=1;i<=quant;i++)
    {
        float x;
        float y;
        float radius = GetRandomValue(5,10);//5;
        if(createMouse==1)
        {
            x = GetMouseX();
            y = GetMouseY();
        }else
        {
            x = GetRandomValue(offsetX,ScreenX-offsetX-radius);
            y = GetRandomValue(offsetY,Floor-radius); 
        }
        
        float velx = GetRandomValue(-150,150);
        float vely = 0;
        Balls.emplace_back(i,x,y,velx,vely,radius);
    }
}



void resolveColision(Ball &obj1,Ball &obj2,float dx,float dy,float dist,float radius)
{
    float nx = dx / dist; //normalX
    float ny = dy / dist; //normalY
    float dsx = obj1.vel.x - obj2.vel.x; //diferença de velocidadeX
    float dsy = obj1.vel.y - obj2.vel.y; //diferença de velocidadeY
    float dot = (dsx * nx) + (dsy * ny); //formula DOT
    
    if(dot>=0)
    {
        return;
    }
    
    float impulso = -(1.0f + Fricton) * dot;
    impulso /= 2.0f;

    obj1.vel.x += impulso * nx;
    obj1.vel.y += impulso * ny;

    obj2.vel.x -= impulso * nx;
    obj2.vel.y -= impulso * ny;
    
    if(dist < radius && dist > 0)
    {
        float overlap = radius - dist;
        obj1.pos.x += nx * overlap *0.5;
        obj1.pos.y += ny * overlap *0.5;

        obj2.pos.x -= nx * overlap *0.5;
        obj2.pos.y -= ny * overlap *0.5;
    }
    
}

void CheckColision(Ball &obj1,Ball &obj2)
{
    if(obj1.pos.x==obj2.pos.x && obj1.pos.y==obj2.pos.y)
    {
        return;
    }

    float dx = obj1.pos.x-obj2.pos.x;
    float dy = obj1.pos.y-obj2.pos.y;
    float dist = sqrt((dx)*(dx)+(dy)*(dy));
    float radius = obj1.radius+obj2.radius;

    if(dist<radius&&dist>0)
    {
        resolveColision(obj1,obj2,dx,dy,dist,radius);
    }
}

vector<Ball*> buscaBallsGrid(vector<Ball> &ball, Vector2 grid)
{
    vector<Ball*> encontrados;

    for(Ball &obj : ball)
    {
        if(FloatEquals(obj.Grid.x,grid.x) && FloatEquals(obj.Grid.y,grid.y))
        {
            encontrados.emplace_back(&obj);
        }
    }
    return encontrados;
}

void checkColisionGrid(Ball& ball1)
{
    for(int dx=-1;dx<=1;dx++)
    {
        for(int dy=-1;dy<=1;dy++)
        {
            Vector2 grid = {float(ball1.Grid.x+dx),float(ball1.Grid.y+dy)};
            vector<Ball*> encontrados = buscaBallsGrid(Balls,grid);
            for(Ball* ball2 : encontrados)
            {
                CheckColision(ball1,*ball2);
            }
        }
    }
}

void checkColisonAll()
{
    for(size_t i = 0; i < Balls.size();i++)
    {
        for(size_t j = i + 1; j < Balls.size();j++)
        {
            CheckColision(Balls[i],Balls[j]);
        }
    }
}

void checkGridPos(Ball &ball)
{

    float relativeX = ball.pos.x-offsetX;
    float relativeY = ball.pos.y-offsetY;

    ball.Grid.x = static_cast<int>(floor(relativeX/GridTam));
    ball.Grid.y = static_cast<int>(floor(relativeY/GridTam));
    //GridList[int(ball.Grid.y)][int(ball.Grid.x)] = &ball; 
}

void UpdatePhysics(float dt)
{
    for(Ball& ball : Balls)
    {
        ball.acel.y=Gravity;

        ball.vel.x+=ball.acel.x * dt;
        ball.vel.y+=ball.acel.y * dt;

        ball.pos.x+=ball.vel.x * dt;
        ball.pos.y+=ball.vel.y * dt;

        if(ball.pos.x+ball.radius>=ScreenX-offsetX)
        {
            ball.pos.x=ScreenX-offsetX-ball.radius;
            ball.vel.x=ball.vel.x*-1;
        }
        if(ball.pos.x-ball.radius<=offsetX)
        {
            ball.pos.x=offsetX+ball.radius;
            ball.vel.x=ball.vel.x*-1;
        }

        if(ball.pos.y+ball.radius>=ScreenY-offsetY)
        {
            ball.pos.y=ScreenY-offsetY-ball.radius;
            ball.vel.y=(ball.vel.y*-1)*Fricton;
        }
        if(ball.pos.y-ball.radius<=offsetY)
        {
            ball.pos.y=offsetY+ball.radius;
            ball.vel.y=(ball.vel.y*-1)*Fricton;
        }

        checkGridPos(ball);
    }


    checkColisonAll();
    //for(Ball &obj : Balls){ checkColisionGrid(obj); }

}

void UpdatePhysicsSubSteps(float dt, int subSteps)
{
    float Subdt = dt/float(subSteps);
    
    for(int i = subSteps;i--;)
    {
        UpdatePhysics(Subdt);
    }
}

void DrawCircleInScreen()
{
    for(const Ball& ball : Balls)
    {
        DrawCircle(
            static_cast<int>(ball.pos.x),
            static_cast<int>(ball.pos.y),
            ball.radius,
            //ball.ID==1 ? BLACK : ball.color
            ball.color
        );
    }
    
}

void DrawGrid()
{
    int quantGridX = (ScreenX-2*offsetX)/GridTam;
    int quantGridY = (ScreenY-2*offsetY)/GridTam;

    for(int i = 0;i<quantGridX;i++)
    {
        for(int j = 0;j<=quantGridY;j++)
        {
            DrawRectangleLines((i*GridTam)+offsetX,(j*GridTam)+offsetY,GridTam,GridTam,BLACK);
        }
    }
}

void DrawScene(int FPS)
{
    DrawRectangle(0,static_cast<int>(Floor),ScreenX,ScreenY-static_cast<int>(Floor),DARKGRAY);
    DrawText(TextFormat("FPS: %i",FPS), 10,10,20, BLACK);
    DrawText(TextFormat("Objetos: %i",static_cast<int>(Balls.size())),110,10,20,BLACK);
}

void CreateBall60(int FPS)
{
    if(FPS>60)
    {
        CreateBalls(1,0);
    }
}

int main()
{
    InitWindow(ScreenX,ScreenY,"fisica bolistica");

    //CreateBalls(Quant,0);

    while(!WindowShouldClose())
    {
        int FPS = GetFPS();
        float DT = GetFrameTime();
        UpdatePhysicsSubSteps(DT,5);

        CreateBall60(FPS);

        if(IsKeyPressed(KEY_SPACE))
        {
            CreateBalls(1,1);
        }
        

        BeginDrawing();
        ClearBackground(WHITE);
        DrawCircleInScreen();
        DrawScene(FPS);
        //DrawGrid();
        EndDrawing();   
    }
    CloseWindow();
    return 0;
}