#ifndef DATA_PLOT2_H
#define DATA_PLOT2_H

#include <qwt-qt4/qwt_plot.h>

#define PLOT_SIZE 201      // 0 to 200

class DataPlot2 : public QwtPlot
{
    Q_OBJECT

public:
    DataPlot2(QWidget* = NULL);
    void addData(int accelerometer_x, int accelerometer_y, int accelerometer_z);

private:
    void alignScales();

    double d_t[PLOT_SIZE];
    double d_x[PLOT_SIZE];
    double d_y[PLOT_SIZE];
    double d_z[PLOT_SIZE];

};

#endif // DATA_PLOT2_H
