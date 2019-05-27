#include "pch.h"
#include "Contour.h"

using namespace std;


namespace ContourHelpers
{

	/*bool Point::operator==(const Point& p1, const Point& p2)
	{
		return p1.X == p2.X && p1.Y == p2.Y;
	}*/

	//	bool Point::operator ==(const Point& point) {
	//		if (this->X == point.X && this->Y == point.Y)
	//			return true;
	//		return false;
	//	}

	Contour::Contour()
	{

	}

	Contour::Contour(ContourType type)
	{
		Type = type;
	}

	Contour::Contour(unsigned char color)
	{
		m_Color = color;
	}

	Contour::Contour(Point* contourPoints, int pointsNumber)
	{
		for (int i = 0; i < pointsNumber; i++)
			AddPoint(contourPoints[i]);
	}

	Contour::~Contour()
	{
		for (unsigned int y = 0; y < m_PointsMap.size(); y++)
			delete m_PointsMap[y];
	}

	Point* Contour::operator[](unsigned int i)
	{
		if (i > m_Points.size()) {
			return &m_Points[0];
		}
		return &m_Points[i];
	}

	void Contour::AddPoint(Point point)
	{
		m_Points.push_back(point);
		if (m_PointsMap.count(point.Y) == 0)
		{
			map<int, vector<int>*>* newXMap = new map<int, vector<int>*>();
			vector<int>*  newXvector = new vector<int>();
			newXvector->push_back(Length);
			newXMap->insert(make_pair(point.X, newXvector));
			m_PointsMap.insert(make_pair(point.Y, newXMap));
		}
		else
		{
			map<int, vector<int>*>* XMap = m_PointsMap[point.Y];
			vector<int>*  Xvector = nullptr;
			if (XMap->count(point.X) == 0)
			{
				Xvector = new vector<int>();
				Xvector->push_back(Length);
				XMap->insert(make_pair(point.X, Xvector));
			}
			else
			{
				XMap->at(point.X)->push_back(Length);
			}
		}
		++Length;
	}

	Point* Contour::GetPoint(int i)
	{
		return &m_Points[i];
	}

	unsigned char Contour::GetColor()
	{
		return m_Color;
	}


	int Contour::Size()
	{
		return Length;
	}

	/*
		Находит ближайшую слева точку контура лежащую в той же строке что и заданная
	*/
	Point* Contour::FindLeftNearestPoint(int pointnumber)
	{
		int LeftNearestPointIndex = -1;
		for (auto pointNumbers : *m_PointsMap[m_Points[pointnumber].Y])
		{
			if (pointNumbers.first < m_Points[pointnumber].X)
				LeftNearestPointIndex = (*pointNumbers.second)[0];
			else
				break;
		}
		if (LeftNearestPointIndex >=0 )
			return &m_Points[LeftNearestPointIndex];
		return nullptr;
	}

	Point* Contour::FindRightNearestPoint(int pointnumber)
	{
		for (auto pointNumbers : *m_PointsMap[m_Points[pointnumber].Y])
		{
			if (pointNumbers.first > m_Points[pointnumber].X)
				return &m_Points[(*pointNumbers.second)[0]];
		}
		return nullptr;
	}

	/*Point* Contour::GetNearestContourPoint(Point* point, Contour::SearchNearestPointDirection direction)
	{
		int lastDistance = MAXINT;
		Point *p = nullptr;
		for (int i = 0; i < this->Length; i++)
		{
			if (m_Points[i].Y == point->Y)
			{
				if (m_Points[i].X == point->X)
					continue;

				int newDistance = m_Points[i].X - point->X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}
*/
	Point* Contour::GetNearestContourPoint(int pointIndex, SearchNearestPointDirection direction)
	{
		int lastDistance = direction == Right ? MAXINT : MININT;
		Point *p = nullptr;
		
		int prevPoint = GetNextContourPointIndex(pointIndex);
		int nextPoint = GetPrevContourPointIndex(pointIndex);
		
		// Если предыдущая и следующая точки контура имеют одинаковое значение
		// координаты Y (лежат в одной строке), это означает что точка с номером
		// pointIndex лежит на пике контура обращённом вверх или вниз
		// Если пик обращён внутрь контура то выполняем поиск точки контура 
		// лежащей справа или слева от точки с номером pointIndex
		// если пик направлен во вне контура возвращаем null

		if ((m_Points[prevPoint].Y - m_Points[nextPoint].Y) == 0)
		{
			// Имеем пик
			if (m_Points[prevPoint].Y > m_Points[pointIndex].Y)
			{
				// Пик направленн вверх
				// Пик направлен внутрь контура если точка НАД пиком 
				// лежит внутри контура или принадлежит контуру

				if (!ContainsPoint(m_Points[pointIndex].X, m_Points[pointIndex].Y - 1)
					&&
					!PointLaysOnContour(m_Points[pointIndex].X, m_Points[pointIndex].Y - 1)
					)
					return nullptr;

			}
			if (m_Points[prevPoint].Y < m_Points[pointIndex].Y)
			{
				// Пик направленн вниз
				// Пик направлен внутрь контура если точка ПОД пиком 
				// лежит внутри контура или принадлежит контуру

				// точка под пиком
				if (!ContainsPoint(m_Points[pointIndex].X, m_Points[pointIndex].Y + 1)
					&&
					!PointLaysOnContour(m_Points[pointIndex].X, m_Points[pointIndex].Y + 1)
					)
					return nullptr;
			}
			if (m_Points[prevPoint].Y == m_Points[pointIndex].Y)
			{
				// Имеем плато из трёх точек (минимум)
				switch (direction)
				{
				case ContourHelpers::Contour::Left:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[prevPoint] : &m_Points[nextPoint];
				case ContourHelpers::Contour::Right:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[nextPoint] : &m_Points[prevPoint];
				default:
					break;
				} 
			}
		}

		// В цикле просмотрим все точки контура и выберем ближайшую 
		// к точке с номером pointIndex
		for (int i = 0; i < this->Length; i++)
		{
			// Исключим из рассмотрения точку с номером pointIndex
			if (i == pointIndex)
				continue;

			if (m_Points[i].Y == m_Points[pointIndex].Y)
			{

				int newDistance = m_Points[i].X - m_Points[pointIndex].X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}

	/*Point* Contour::GetNearestInternalContourPoint(Point* externalContourPoint, SearchNearestPointDirection direction)
	{
		int lastDistance = direction == Right ? MAXINT : MININT;
		Point *p = nullptr;
		for (int i = 0; i < Length; i++)
		{
			if (m_Points[i].Y == externalContourPoint->Y)
			{
				int newDistance = m_Points[i].X - externalContourPoint->X;
				switch (direction)
				{
				case Right:
					if (newDistance >= 0 && newDistance < lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				case Left:
					if (newDistance <= 0 && newDistance > lastDistance)
					{
						lastDistance = newDistance;
						p = &m_Points[i];
					}
				}
			}
		}
		return p;
	}
*/
	/*
	  Находит номер точки в m_Points ближайшей по горизонтали точки контура лежащей
	  справа от точки, заданой координатами x, y
	  Входные параметры
			x, y - координаты точки для которой нужно найти
					ближайшую справа точку контура
	  Возвращаемое значение
			номер точки в векторе m_Points если она найдена
			-1 если точка не найдена
	*/
	int Contour::GetRightNearestPointIndex(int x, int y)
	{
		int index = -1;
		int lastDistance = MAXINT;

		for (int i = 0; i < Length; i++)
		{
			if (m_Points[i].Y == y)
			{
				// Исключим из рассмотрения точку с координатами x,y
				if (m_Points[i].X == x)
					continue;

				int newDistance = m_Points[i].X - x;
				if (newDistance >= 0 && newDistance < lastDistance)
				{
					lastDistance = newDistance;
					index = i;
				}
			}
		}
		return index;
	}

	/*
	Возвращает индекс следующей точки контура. 
	Для последней точки контура следующей считается точка с номером 0
	Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetNextContourPointIndex(int i)
	{
		return ++i % Length;
	}

	/*
	 Возвращает индекс предыдущей точки контура
	 Для точки контура с номером 0 предыдущей считается последняя точка контура
	 Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetPrevContourPointIndex(int i)
	{
		return (--i + Length) % Length;
	}


	bool Contour::Contains(Point* point)
	{
		for (Point p : m_Points)
		{
			if (p.X == point->X && p.Y == point->Y)
				return true;
		}
		return false;
	}

	bool Contour::PointLaysOnContour(int x, int y)
	{
		if ((m_PointsMap.count(y) != 0) && (m_PointsMap[y]->count(x) != 0))
			return true;
		return false;
	}

	/*
	Проверяет лежит ли заданная точка внутри контура
	Входные параметры
		x, y - координаты проверяемой точки
	Возвращаемое значение
		true - если точка лежит внутри контура
		false -  если точка является точкой контура или лежит вне контура

		Проверка того что точка лежи внутри контура выполняется следующим образом
		из проверяемой точки вправо проводится отрезок прямой до границы изображения.
		Подсчитывается количество пересечений проведённого отрезка с контуром.
		Если число пересечений нечётное то точка лежит внутри контура.
		Если число пересечений чётное то точка лежит вне контура.

		Считается что отрезок прямой пересекает линию контура в точке Point[i]
		если точки Point[i-1] и Point[i+1] лежат по разные стороны проведённого
		горизонтального отрезка.

		Если точка Point[i-1] лежит на проведённом отрезке то просматриваем контур в
		направлении убывания номеров точек контура пока не обнаружим точку с координатой Y
		отличающейся от координаты Y проверяемой точки. 
		Если найденная точка Point[i-n] и точка Point[i+1] лежат по разные стороны отрезка, то отрезок
		пересекает контур в точке Point[i].

		Если точка Point[i+1] лежит на проведённом отрезке то просматриваем контур в
		направлении возрастания номеров точек контура пока не обнаружим точку с координатой Y
		отличающейся от координаты Y проверяемой точки.
		Если найденная точка Point[i+n] и точка Point[i-1] лежат по разные стороны отрезка, то отрезок
		пересекает контур в точке Point[i].

		В дальнейшем точки Point[i+n] и Point[i-n] будем называть Point[i+1] и Point[i-1]
		соответственно.

		если точки Point[i-1] и Point[i+1] лежат по одну сторону отрезка то считаем, что
		отрезок не пересекает линию контура.

		Если существует несколько точек контура с координатами X,Y предполагаемой
		точки пересечения отрезка с линией контура, то определяем факт пересечения отрезком
		линии контура для каждой такой точки отдельно.

	*/
	
	bool Contour::ContainsPoint(int x, int y)
	{
		// Проверим принадлежность точки контуру
		if ( (m_PointsMap.count(y) != 0) && (m_PointsMap[y]->count(x) !=0) )
			return false;

		//  Если среди точек контура нет точек с координатой Y равной y - точка x, y лежит за пределами контура
		if (m_PointsMap.count(y) == 0)
			return false;
		
		int contourCrossingCount = 0;
		// Сформируем список номеров точек контура с Y = y, лежащих справа от точки с координатами x, y.
		vector<int> points;
		for (auto v : *m_PointsMap[y])
		{
			if (v.first > x)
				for (int i : *v.second)
					points.push_back(i);
		}

		for (int currentPointIndex : points )
		{
			// Получим индексы точек перед и после текущей точки 
			int prevPointIndex = GetPrevContourPointIndex(currentPointIndex);
			int nextPointIndex = GetNextContourPointIndex(currentPointIndex);

			// Сравним координаты Y точек соседних с текущей 
			if (m_Points[prevPointIndex].Y == m_Points[currentPointIndex].Y)
			{
				// Если предыдущая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
				// по контуру назад пока не найдётся точка лежащая выше или ниже текущей точки.
				while (m_Points[prevPointIndex].Y == m_Points[currentPointIndex].Y)
					prevPointIndex = GetPrevContourPointIndex(prevPointIndex);
			}

			if (m_Points[nextPointIndex].Y == m_Points[currentPointIndex].Y)
			{
				// Если следующая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
				// по контуру вперёд пока не найдётся точка лежащая выше или ниже текущей точки.
				while (m_Points[nextPointIndex].Y == m_Points[currentPointIndex].Y)
					nextPointIndex = GetPrevContourPointIndex(nextPointIndex);
			}

			if (abs(m_Points[prevPointIndex].Y - m_Points[nextPointIndex].Y) == 2)
			{
				// Если разность координат Y предыдущей и последующей точки равна 2 , это означает, что
				// предыдущая и следующая точка контура лежат по разные стороны горизонтали на которой лежит
				// найденная ближайшая справа точка контура, то есть горизонталь проведённая через ближайшую
				// справа точку пересекает контур в этой точке
				//		увеличиваем счётчик пересечений контура на 1
				++contourCrossingCount;
				// устанавливаем координату X найденной точки в качестве новой начальной координаты для поиска
				// следующей ближайшей точки контура справа
				//		x = m_Points[currentPointIndex].X;
			}
		}
		return contourCrossingCount % 2 == 1 ? true : false;
	}
	
	vector<int>* Contour::GetRightPoints(int x, int y)
	{
		map<int, map<int, vector<int>*>* >::iterator  it = m_PointsMap.begin();

		map<int, vector<int>*>* xMap = m_PointsMap[y];

		for (auto N : *m_PointsMap[y])
		{ 
			auto i = N;
		}

		return nullptr;
	}

}  // namespace ContourHelpers
