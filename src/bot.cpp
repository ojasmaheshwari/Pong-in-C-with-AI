

float predictBallFinalPosition(float x_i, float x_f, float v_y, float v_x, float y_i) 
{
    float delta_x = x_f - x_i;
    float tan_theta = v_y / v_x;
    return delta_x * tan_theta + y_i;
}