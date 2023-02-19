#include "pch.h"
#include "ColorGroup.h"
#include "Color.h"


namespace ContourExtractorWindowsRuntimeComponent
{
	ColorGroup::ColorGroup()
	{

	}

	ColorGroup::ColorGroup(unsigned char* imgData, int bufferLength)
	{
		for (int pos = 0; pos < bufferLength; pos += 4)
		{
			Color color = { imgData[pos + 0] , imgData[pos + 1], imgData[pos + 2], 0xFF };
			m_color_values.push_back(color);
		}
		CalcColorGroupParams();
	}

	ColorGroup::ColorGroup(std::vector<Color>* color_values)
	{

	}

	unsigned char ColorGroup::getBaseMaxColorRange()
	{
		return m_maxColorRange;
	}

	Color ColorGroup::getMidleGroupColor()
	{
		Color color;
		return color;
	}

	Color ColorGroup::getMedianGroupColor()
	{
		Color color;
		return color;
	}
	
	Color ColorGroup::getAverageGroupColor()
	{
		Color color;
		long r = 0, g = 0, b = 0;
		for (Color c : m_color_values)
		{
			r += c.red;
			g += c.green;
			b += c.blue;

		}

		int l = (int)m_color_values.size();
		color.red = (unsigned char)(r / l);
		color.green = (unsigned char)(g / l);
		color.blue = (unsigned char)(b / l);
		return color;
	}

	void ColorGroup::Split(ColorGroup* group1, ColorGroup* group2)
	{
		// Sort image color data by color with max range
		switch (m_MaxRangeColor)
		{
		case BaseColor::Red:
			std::sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.red < b.red; });
			break;
		case BaseColor::Green:
			std::sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.green < b.green; });
			break;
		case BaseColor::Blue:
			std::sort(m_color_values.begin(), m_color_values.end(), [](Color& a, Color& b) { return a.blue < b.blue; });
			break;
		}

		int median = (int)(m_color_values.size() / 2);
	
		std::vector<Color> color_values;
		for (int i = 0; i < median; i++)
			color_values.push_back(m_color_values[i]);
		group1->AddColorValues(&color_values);

		color_values.clear();

		for (int i = median; i < m_color_values.size(); i++)
			color_values.push_back(m_color_values[i]);
		group2->AddColorValues(&color_values);
	}

	

	void ColorGroup::AddColorValues(std::vector<Color> *color_values)
	{
		m_color_values = *color_values;
		CalcColorGroupParams();
	}

	bool ColorGroup::Contain(unsigned char r, unsigned char g, unsigned char b)
	{
		if (r >= m_r_min && r <= m_r_max && g >= m_g_min && g <= m_g_max && b >= m_b_min && b <= m_b_max)
			return true;

		return false;
	}

	void ColorGroup::CalcColorGroupParams()
	{
		m_r_min = 255;
		m_r_max = 0;
		m_g_min = 255;
		m_g_max = 0;
		m_b_min = 255;
		m_b_max = 0;

		// Store max and min value for each color
		for (Color color : m_color_values)
		{
			m_r_min = color.red   < m_r_min ? color.red   : m_r_min;
			m_r_max = color.red   > m_r_max ? color.red   : m_r_max;
			m_g_min = color.green < m_g_min ? color.green : m_g_min;
			m_g_max = color.green > m_g_max ? color.green : m_g_max;
			m_b_min = color.blue  < m_b_min ? color.blue  : m_b_min;
			m_b_max = color.blue  > m_b_max ? color.blue  : m_b_max;
		}

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