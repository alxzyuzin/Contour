#include "pch.h"
#include "ColorRegion.h"
#include "Color.h"

namespace ContourExtractorWindowsRuntimeComponent
{
	ColorRegion::ColorRegion()
	{

	}

	ColorRegion::ColorRegion(unsigned char* imgData, int bufferLength)
	{
		for (int pos = 0; pos < bufferLength; pos += 4)
		{
			m_r_values.push_back(imgData[pos + 0]);
			m_g_values.push_back(imgData[pos + 1]);
			m_b_values.push_back(imgData[pos + 2]);
		}
		std::sort(m_r_values.begin(), m_r_values.end());
		std::sort(m_g_values.begin(), m_g_values.end());
		std::sort(m_b_values.begin(), m_b_values.end());

		CalcColorRegionParams();
		
	}

	ColorRegion::ColorRegion(std::vector<unsigned char>* r_values, std::vector<unsigned char>* g_values, std::vector<unsigned char>* b_values)
	{
		m_r_values = *r_values;
		m_g_values = *g_values;
		m_b_values = *b_values;

		CalcColorRegionParams();
	}
	void ColorRegion::setColorValues(std::vector<unsigned char>* r_values, std::vector<unsigned char>* g_values, std::vector<unsigned char>* b_values)
	{
		m_r_values = *r_values;
		m_g_values = *g_values;
		m_b_values = *b_values;

		CalcColorRegionParams();
	}

	unsigned char ColorRegion::getBaseMaxColorRange()
	{
		return m_maxBaseColorRange;
	}
	
	

	Color ColorRegion::getMidleRegionColor()
	{
		Color c;
		c.red   = m_r_min + (m_r_max - m_r_min) / 2;
		c.green = m_g_min + (m_g_max - m_g_min) / 2;
		c.blue  = m_b_min + (m_b_max - m_b_min) / 2;
		return c;
	}

	Color ColorRegion::getMediandRegionColor()
	{
		Color c;
		c.red =   m_r_values[m_r_values.size() / 2];
		c.green = m_g_values[m_g_values.size() / 2];
		c.blue =  m_b_values[m_b_values.size() / 2];

		
		return c;
	}

	Color ColorRegion::getAverageRegionColor()
	{
		Color c;
		long R = 0, G = 0, B = 0;

		for (unsigned char r : m_r_values)
			R += r;
		c.red = (unsigned char) (R / m_r_values.size());

		for (unsigned char g : m_g_values)
			G += g;
		c.green = (unsigned char) (G / m_g_values.size());
		
		for (unsigned char b : m_b_values)
			B += b;
		c.blue = (unsigned char)(B / m_b_values.size());


		return c;
	}

	void ColorRegion::Split(ColorRegion *reg1, ColorRegion *reg2)
	{
		int vectorMedian;
		
		std::vector<unsigned char> s1_values;
		std::vector<unsigned char> s2_values;
		
		
		switch (m_MaxRangeColor)
		{
		case BaseColor::Red:
			vectorMedian = int(m_r_values.size() / 2);

			for (int i = 0; i < vectorMedian; i++)
				s1_values.push_back(m_r_values[i]);
			for (int i = vectorMedian; i < m_r_values.size(); i++)
				s2_values.push_back(m_r_values[i]);

				reg1->setColorValues(&s1_values, &m_g_values, &m_b_values);
				reg2->setColorValues(&s2_values, &m_g_values, &m_b_values);
		
			break;
		case BaseColor::Green:
			vectorMedian = int(m_g_values.size() / 2);

			for (int i = 0; i < vectorMedian; i++)
				s1_values.push_back(m_g_values[i]);
			for (int i = vectorMedian; i < m_g_values.size(); i++)
				s2_values.push_back(m_g_values[i]);
			
			reg1->setColorValues(&m_r_values, &s1_values, &m_b_values);
			reg2->setColorValues(&m_r_values, &s2_values, &m_b_values);
			break;
		case BaseColor::Blue:
			vectorMedian = int(m_b_values.size() / 2);

			for (int i = 0; i < vectorMedian; i++)
				s1_values.push_back(m_b_values[i]);
			for (int i = vectorMedian; i < m_b_values.size(); i++)
				s2_values.push_back(m_b_values[i]);

			reg1->setColorValues(&m_r_values, &m_g_values, &s1_values);
			reg2->setColorValues(&m_r_values, &m_g_values, &s2_values);
			break;
		}
	}

	BaseColor ColorRegion::SplitColor(std::vector<unsigned char> *range1, std::vector<unsigned char>* range2)
	{

		//int vectorMedian;
		/*
		switch (m_MaxRangeColor)
		{
		case BaseColor::Red:
			vectorMedian = m_r_values.size() / 2;

			for (int i = 0; i < vectorMedian; i++)
				range1->push_back(m_r_values[i]);
			for (int i = vectorMedian; i < m_r_values.size(); i++)
				range2->push_back(m_r_values[i]);

			return BaseColor::Red;
		case BaseColor::Green:
			vectorMedian = m_g_values.size() / 2;

			for (int i = 0; i < vectorMedian; i++)
				range1->push_back(m_g_values[i]);
			for (int i = vectorMedian; i < m_g_values.size(); i++)
				range2->push_back(m_g_values[i]);

			return BaseColor::Green;
		case BaseColor::Blue:
			vectorMedian = m_b_values.size() / 2;

			for (int i = 0; i < vectorMedian; i++)
				range1->push_back(m_b_values[i]);
			for (int i = vectorMedian; i < m_b_values.size(); i++)
				range2->push_back(m_b_values[i]);

	   	   return BaseColor::Blue;
		}
		*/
		return BaseColor::Red;
	}

	void ColorRegion::CalcColorRegionParams()
	{

		// Store max and min value for each color
		m_r_min = m_r_values[0];
		m_r_max = m_r_values[m_r_values.size() - 1];
		m_g_min = m_g_values[0];
		m_g_max = m_g_values[m_g_values.size() - 1];
		m_b_min = m_b_values[0];
		m_b_max = m_b_values[m_b_values.size() - 1];


		// Calculate what color has maximum range and what is the value of this range
		if ((m_r_max - m_r_min) > (m_g_max - m_g_min))
		{
			m_maxBaseColorRange = m_r_max - m_r_min;
			m_MaxRangeColor = BaseColor::Red;
		}
		else
		{
			m_maxBaseColorRange = m_g_max - m_g_min;
			m_MaxRangeColor = BaseColor::Green;
		};

		if ((m_b_max - m_b_min) > (m_g_max - m_g_min))
		{
			m_maxBaseColorRange = m_b_max - m_b_min;
			m_MaxRangeColor = BaseColor::Blue;
		}
		else
		{
			m_maxBaseColorRange = m_g_max - m_g_min;
			m_MaxRangeColor = BaseColor::Green;
		};
	}

	bool ColorRegion::Contain(unsigned char r, unsigned char g, unsigned char b)
	{
		if ((r >= m_r_min) && (r <= m_r_max) &&
			(g >= m_g_min) && (g <= m_g_max) &&
			(b >= m_b_min) && (b <= m_b_max))
			return true;
		return false;
	}
}
