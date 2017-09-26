extern float errotheta;
extern float dist;

class target {
public:
	float x;
	float y;
};

class robot{

public:
	int id;
	float x;
	float y;
	float theta;
	void controlador_angulo(target obj, int& vel_righ, int& vel_left);
	void controlador_dist(target obj, int& vel_righ, int& vel_left);
};
