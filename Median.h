// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// Median.h - Median widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef MEDIAN_H
#define MEDIAN_H

#include "ImageFilter.h"

class Median : public ImageFilter {
	Q_OBJECT

public:
	Median				(QWidget *parent = 0);	// constructor
	QGroupBox*	controlPanel	();			// create control panel
	bool		applyFilter	(ImagePtr, ImagePtr);	// apply filter to input
	void		reset		();			// reset parameters
	void		median		(ImagePtr, int, ImagePtr);

protected slots:
	void		changeSize	(int);
	void		changeItrs	(int);

private:
	// widgets
	QSlider*	m_slider [2];	// Median sliders
	QSpinBox*	m_spinBox[2];	// Median spin boxes
	QGroupBox*	m_ctrlGrp;	// groupbox for panel
};

#endif	// MEDIAN_H
