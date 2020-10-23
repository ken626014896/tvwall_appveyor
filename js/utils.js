function calc_dest_area(x, y, s_width, s_height, p_width, p_height){
    for(var i=0; i<$room.phy_wall_column; i++){
        for(var j=0; j<$room.phy_wall_row; j++){
            var r_width = (p_width * 1.0) / $room.phy_wall_column;
            var r_height = (p_height * 1.0) /$room.phy_wall_row;
            if(r_width == 0 || r_height == 0){
                continue;
            }

            for(var a=0;a<$manager.logical_wall_column;a++){
                for(var b=0;b<$manager.logical_wall_row;b++){
                    var l_width = (r_width * 1.0) / $manager.logical_wall_column;
                    var l_height = (r_height * 1.0) /$manager.logical_wall_row;
                    if(l_width == 0 || l_height == 0){
                        continue;
                    }
                    var l_x_begin = r_width*i + l_width*a;
                    var l_x_end = r_width*i + l_width*(a+1);
                    var l_y_begin = r_height*j + l_height*b;
                    var l_y_end = r_height*j + l_height*(b+1);
                    if(x <= 0){
                        destArea.x = 0;
                        destArea.column = 0;
                    }
                    else if(x >= l_x_begin && x <l_x_end){
                        destArea.column = (i + 1) * a
                        destArea.x = l_x_begin;
                    }
                    if(y <= 0){
                        destArea.y = 0;
                        destArea.row = 0;
                    }
                    else if(y >= l_y_begin && y < l_y_end){
                        destArea.row = (j+1)*b;
                        destArea.y = l_y_begin;
                    }

                    var drag_x_end = x + s_width;
                    if(drag_x_end > p_width){
                        destArea.width = p_width - destArea.x;
//                        console.log("column num full", i+1, j+1, a+1, b+1, destArea.column);
                        destArea.column_num = $room.phy_wall_column * $manager.logical_wall_column - destArea.column;
                    }
                    else if(drag_x_end >= l_x_begin && drag_x_end <l_x_end){
                        if(l_x_end - destArea.x <  s_width){
                            destArea.width = s_width;
                            destArea.column_num = 1;
                        }
                        else{
                            destArea.width = l_x_end - destArea.x;
//                            console.log("column num", i+1, j+1, a+1, b+1, destArea.column);
                            destArea.column_num = i * $manager.logical_wall_column + a + 1 - destArea.column;
                        }
                    }

                    var drag_y_end = y + s_height;
                    if(drag_y_end > p_height){
                        destArea.height = p_height - destArea.y;
//                        console.log("row num full", i+1, j+1, a+1, b+1, destArea.row);
                        destArea.row_num = $room.phy_wall_row * $manager.logical_wall_row - destArea.row;
                    }
                    else if(drag_y_end >= l_y_begin && drag_y_end <l_y_end){
                        if(l_y_end - destArea.y < s_height){
                            destArea.height = s_height;
                            destArea.row_num = 1;
                        }
                        else{
                            destArea.height = l_y_end - destArea.y;
//                            console.log("row num", i+1, j+1, a+1, b+1, destArea.row);
                            destArea.row_num = j * $manager.logical_wall_row + b + 1  - destArea.row;
                        }
                    }
                }
            }

        }
    }
}
function calc_dest_area_new(x, y, s_width, s_height, p_width, p_height){  //缩放到小屏方法
    //信号模块的x,y width,height  和C区的width，height、
    var r_width = (p_width * 1.0) / $room.phy_wall_column; //小屏的宽
    var r_height = (p_height * 1.0) /$room.phy_wall_row;


//       display_signal_source.old_x=x
//       display_signal_source.old_y=y
//       display_signal_source.old_width=s_width
//       display_signal_source.old_height=s_height
        var gravity_x=x+s_width/2;  //信号的重心
        var gravity_y=y+s_height/2;
        var temp=0;
        var posX=0;
        var posY=0;
        for(let i=0;i<= $room.phy_wall_column;i++)
        {

            let xx=i*r_width;
            if(gravity_x>=temp&&gravity_x<=xx){
                posX=i-1;
                break;
            }
            temp=xx;
        }
        temp=0;
        for(let i=0;i<=$room.phy_wall_row;i++)
        {

            let yy=i*r_height;
            if(gravity_y>=temp&&gravity_y<=yy){
                posY=i-1;
                break;
            }
            temp=yy;
        }
//       console.log(posX+","+posY)
        destArea.x=posX*r_width
        destArea.y=posY*r_height
        destArea.width=r_width
        destArea.height=r_height


}

function full_screen(p_width, p_height){
    destArea.x=0
    destArea.y=0
    destArea.width=p_width
    destArea.height=p_height

}

