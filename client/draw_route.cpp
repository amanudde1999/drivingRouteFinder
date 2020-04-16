#include "draw_route.h"
#include "map_drawing.h"
#include "consts_and_types.h"
#include "map_drawing.h"
#define  BLACK 0x0000
extern shared_vars shared;

void draw_route() {
	status_message("Line will be drawn now!");
	// If there are any waypoints
	if(shared.num_waypoints > 0){
		// We go through all of them
		for(int i =0; i<(shared.num_waypoints-1);i++){
			int32_t x1,x2,y1,y2;
			// Converts the latitude and longitude to coordinates for the screen
			// we take one point
			x1 = longitude_to_x(shared.map_number,shared.waypoints[i].lon);
			y1 = latitude_to_y(shared.map_number,shared.waypoints[i].lat);
			// and one point ahead
			x2 = longitude_to_x(shared.map_number,shared.waypoints[i+1].lon);
			y2 = latitude_to_y(shared.map_number,shared.waypoints[i+1].lat);
			x1-= shared.map_coords.x;
			y1-= shared.map_coords.y;
			x2-= shared.map_coords.x;
			y2-= shared.map_coords.y;
			shared.tft->drawLine(x1,y1,x2,y2,BLACK);	
		}
	}
}
