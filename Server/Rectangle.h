class Rectangle{
private:
  double x;
  double y;
  double width;
  double height;

public:
  Rectangle(double x, double y, double width, double height):
  x(x), y(y), width(width), height(height){}
  Rectangle():x(0), y(0), width(0), height(0){}
  double getWidth(){ return width; }
  double getHeight(){ return height; }
  double getX(){ return x; }
  double getY(){ return y; }
  bool operator==(const Rectangle &other){
    return x == other.x && 
      y == other.y &&
      width == other.width &&
      height == other.height;
  }
};