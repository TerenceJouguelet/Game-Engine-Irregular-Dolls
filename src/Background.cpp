#include "Background.h"
#include "Sheet.h"
#include "System.h"
#include <vector>




Background::Background(string id):AutoMovable(0, 0, id, "data/text/Background.txt"), Movator(id, "data/text/Background.txt"){


}





Background::~Background(){

}
