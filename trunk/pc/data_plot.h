#ifndef _DATA_PLOT_H
#define _DATA_PLOT_H 1

#include <qwt-qt4/qwt_plot.h>

const int PLOT_SIZE = 201;      // 0 to 200

class DataPlot : public QwtPlot
{
    Q_OBJECT

public:
    DataPlot(QWidget* = NULL);
    void addData(int gyro, int u);

private:
    void alignScales();

    double d_x[PLOT_SIZE]; 
    double d_y[PLOT_SIZE]; 
    double d_z[PLOT_SIZE];

};

#endif
