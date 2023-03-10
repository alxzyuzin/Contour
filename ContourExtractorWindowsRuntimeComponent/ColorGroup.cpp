#include "pch.h"
#include "ColorGroup.h"
#include "Color.h"


namespace ContourExtractorWindowsRuntimeComponent
{
	ColorGroup::ColorGroup()
	{

	}

	ColorGroup::ColorGroup(unsigned int* imgData, int bufferLength)
	{
		for (int i = 0; i < bufferLength; i++)
		{
			CharToIntColor color = imgData[i];
			m_color_values.push_back(color.charcolor);
		}
		CalcColorGroupParams();
	}
	
	unsigned char ColorGroup::MaxColorRange()
	{
		return m_maxColorRange;
	}

	unsigned int ColorGroup::AverageGroupColor()
	{
		return m_averageGroupColor.intcolor;
	}
	
	void ColorGroup::Split(ColorGroup* group1, ColorGroup* group2)
	{
		// Sort image color data by color with max range
		switch (m_MaxRangeColor)
		{
		case BaseColor::Red:
			sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.red < b.red; });
			break;
		case BaseColor::Green:
			sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.green < b.green; });
			break;
		case BaseColor::Blue:
			sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.blue < b.blue; });
			break;
		}

		unsigned int median = (unsigned int)(m_color_values.size() / 2);
	
		std::vector<Color> color_values;
		/* 
		for (unsigned int i = 0; i < median; i++)
			color_values.push_back(m_color_values[i]);
		group1->AddColorValues(&color_values);
		color_values.clear();*/
		
		group1->AddColorValuesFromHead(&m_color_values, median);
		
		for (unsigned int i = median; i < m_color_values.size(); i++)
			color_values.push_back(m_color_values[i]);
		group2->AddColorValues(&color_values);
	}

	void ColorGroup::AddColorValues(vector<Color> *color_values)
	{
		m_color_values = *color_values;
		CalcColorGroupParams();
	}

	void ColorGroup::AddColorValuesFromHead(vector<Color>* invector, unsigned int median)
	{
		clock_t start = clock();
		m_color_values.insert(end(m_color_values), begin(*invector), begin(*invector) + median);
		double time = (clock() - start) / (double)(CLOCKS_PER_SEC / 1000);

		CalcColorGroupParams();
	}
	void ColorGroup::AddColorValuesFromTail(vector<Color>* invector, unsigned int median)
	{

	}

	// Define if particular color belongs to this group
	bool ColorGroup::Contains(unsigned char r, unsigned char g, unsigned char b)
	{
		if (r >= m_r_min && r <= m_r_max && g >= m_g_min && g <= m_g_max && b >= m_b_min && b <= m_b_max)
			return true;
		return false;
	}

	bool ColorGroup::Contains(CharToIntColor color)
	{
		if (color.charcolor.red >= m_r_min && color.charcolor.red <= m_r_max &&
			color.charcolor.green >= m_g_min && color.charcolor.green <= m_g_max &&
			color.charcolor.blue >= m_b_min && color.charcolor.blue <= m_b_max)
			return true;
		return false;
	}

	// Calculate max and min value for each color component, max range and color component with max range
	void ColorGroup::CalcColorGroupParams()
	{
		m_r_min = 255;
		m_r_max = 0;
		m_g_min = 255;
		m_g_max = 0;
		m_b_min = 255;
		m_b_max = 0;

		long totalRed = 0;
		long totalGreen = 0;
		long totalBlue = 0;
		long totalColors = 0;

		// Store max and min value for each color
		for (Color color : m_color_values)
		{
			m_r_min = color.red   < m_r_min ? color.red   : m_r_min;
			m_r_max = color.red   > m_r_max ? color.red   : m_r_max;
			m_g_min = color.green < m_g_min ? color.green : m_g_min;
			m_g_max = color.green > m_g_max ? color.green : m_g_max;
			m_b_min = color.blue  < m_b_min ? color.blue  : m_b_min;
			m_b_max = color.blue  > m_b_max ? color.blue  : m_b_max;

			totalRed +=color.red;
			totalGreen += color.green;
			totalBlue += color.blue;
			++totalColors;
		}

		// Calculate group average color
		if (totalColors > 0)
			m_averageGroupColor = { (unsigned char)(totalRed / totalColors), 
									(unsigned char)(totalGreen / totalColors), 
									(unsigned char)(totalBlue / totalColors),
									0xFF
								  };
	
		// Calculate color range for each color component
		unsigned char r_range = m_r_max - m_r_min;
		unsigned char g_range = m_g_max - m_g_min;
		unsigned char b_range = m_b_max - m_b_min;
		// Calculate what color has maximum range and what is the value of this range
		if ((r_range >= g_range ) && (r_range >= b_range))
		{
			m_maxColorRange = r_range;
			m_MaxRangeColor = BaseColor::Red;
		}
		if ((g_range >= r_range) && (g_range >= b_range))
		{
			m_maxColorRange = g_range;
			m_MaxRangeColor = BaseColor::Green;
		}
		if ((b_range >= g_range) && (b_range >= r_range))
		{
			m_maxColorRange = b_range;
			m_MaxRangeColor = BaseColor::Blue;
		}
	}
}