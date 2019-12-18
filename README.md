The are two projects ： QtChartWidget and Splitter, which are introduced in detail as follows.


/************************************ QtChartWidget *********************************************/ 


Imitating displaying multiple real-time curves in qt, by reading datas from a file.

The belowing is the ultimate gui.

![](https://github.com/liming467/QtChartWidget/blob/master/gui.png)

Curve is showed mainly using QChart. It's noted that axisX and axisY have been deprecated. An alternative solution is to use QValueAxis, 

addAxis, and attachAxis. When using the attachAxis function, you need to figure out which axis your series use in case of the appearance 

of unwanted multiple x axes or y axes.

There is a table to display the latest data as well as minimum and maximum value. In the first column of the table, the checkBox controls the visible status of the corresponding curve(the corresponding curve and checkBox have the same color).       

Beside the table, two radioButton in groupBox help to set the status of checkBoxs in table.






/************************************ Splitter *********************************************/ 


The interface is designed as showing the curve of processed 1d data. In the meanwhile, you can see source and result data in two table, respectively. There are several functions as follows.
(1)median filter for 1d data;

(2)average filter for 1d data;

(3)gaussian filter for 1d data;

(4)Fouier transformation for 1d data without acceleration;

(5)fast Fouier transformation for 1d data with zero-padding in most situations;

(6)an example to verify the calculated fft result comparing with the result obtained by matlab(the matlab script file(m file) has been uploaded).   
![](https://github.com/liming467/QtChartWidget/blob/master/actions.png)
![](https://github.com/liming467/QtChartWidget/blob/master/comparation.png)
The comparison between the results calculated by matlab(the left) and this program(the right). After checked, the both results are the same.    



After you run the program, a file dialog will warn you to select a .txt file as source file that will be processed soon. It's suggest that you should select 3.txt in the file of QtChartWidget project, or it might go wrong. Of course, you can modify the codes to satisfy your specific need. 

![](https://github.com/liming467/QtChartWidget/blob/master/path.png)

The picture below is seen firstly, which shows the curve of result data you have already processed.  

![](https://github.com/liming467/QtChartWidget/blob/master/chartView.png)

Another part of tabwidget is as follows. The left table shows the source data and the right for the result data.     

![](https://github.com/liming467/QtChartWidget/blob/master/table.png)




