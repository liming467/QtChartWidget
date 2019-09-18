# QtChartWidget
Imitating displaying multiple real-time curves in qt, by reading datas from a file.

The belowing is the ultimate gui.

![](https://github.com/liming467/QtChartWidget/blob/master/gui.png)

Curves is showed mainly using QChart. It's noted that axisX and axisY have been deprecated. An alternative solution is to use QValueAxis, addAxis, and attachAxis. When using the attachAxis function, you need to figure out which axis your series use in case of the appearance of unwanted multiple x axes or y axes.

There is a table to display the latest data as well as minimum and maximum value. In the first column of the table, the checkBox controls the visible status of the corresponding curve(the corresponding curve and checkBox have the same color).       

Beside the table, two radioButton in groupBox help to set the status of checkBoxs in table.  
