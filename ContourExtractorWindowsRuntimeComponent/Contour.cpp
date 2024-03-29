﻿#include "pch.h"
#include "Contour.h"

using namespace std;


namespace ContourExtractorWindowsRuntimeComponent
{

	bool Point::operator ==(const Point& point)
	{
		return this->X == point.X && this->Y == point.Y;
	}

	Contour::Contour()
	{

	}

	Contour::Contour(ContourType type)
	{
		Type = type;
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

	int Contour::Size()
	{
		return Length;
	}

	/*
		Находит ближайшую слева точку контура лежащую в той же строке что и заданная,
		не совпадающую с заданной точкой.
	*/
	Point* Contour::FindLeftNearestPoint(int pointnumber)
	{
		int Y = m_Points[pointnumber].Y;
		int X = m_Points[pointnumber].X;

		int LeftNearestPointIndex = -1;
		for (auto pointNumbers : *m_PointsMap[Y])
		{
			if (pointNumbers.first < X)
				LeftNearestPointIndex = (*pointNumbers.second)[0];
			else
				break;
		}
		if (LeftNearestPointIndex >= 0)
			return &m_Points[LeftNearestPointIndex];
		return nullptr;
	}

	/*
		Находит ближайшую справа точку контура, лежащую в той же строке что и заданная
		не совпадающую с заданной точкой.
	*/
	Point* Contour::FindRightNearestPoint(int pointnumber)
	{
		for (auto pointNumbers : *m_PointsMap[m_Points[pointnumber].Y])
		{
			if (pointNumbers.first > m_Points[pointnumber].X)
				return &m_Points[(*pointNumbers.second)[0]];
		}
		return nullptr;
	}

	Point* Contour::GetNearestContourPoint(int pointIndex, SearchNearestPointDirection direction)
	{
		int prevPoint = GetNextContourPointIndex(pointIndex);
		int nextPoint = GetPrevContourPointIndex(pointIndex);

		int prevY = m_Points[prevPoint].Y;
		int nextY = m_Points[nextPoint].Y;
		int currY = m_Points[pointIndex].Y;
		// Если предыдущая и следующая точки контура имеют одинаковое значение
		// координаты Y (лежат в одной строке), это означает что точка с номером
		// pointIndex лежит на пике контура обращённом вверх или вниз
		// Если пик обращён внутрь контура то выполняем поиск точки контура 
		// лежащей справа или слева от точки с номером pointIndex
		// если пик направлен во вне контура возвращаем null

		if ((prevY - nextY) == 0)
		{
			// Имеем пик
			int X = m_Points[pointIndex].X;
			int Y = currY;

			// Пик направленн вверх
			// Координаты точки над пиком
			// Пик направлен внутрь контура если точка НАД пиком 
			// лежит внутри контура или принадлежит контуру
			// Если пик направленн вверх и точка над пиком не лежит внутри контура и
			// не лежит на контуре значит ни слева ни справа от точки pointindex не точек
			// контура: возвращаем nullptr
			if (prevY > currY)
				Y = currY - 1;

			// Пик направленн вниз
			// Координаты точки под пиком
			// Пик направлен внутрь контура если точка ПОД пиком 
			// лежит внутри контура или принадлежит контуру
			// Если пик направленн вниз и точка под пиком не лежит внутри контура и
			// не лежит на контуре значит ни слева ни справа от точки pointindex не точек
			// контура: возвращаем nullptr
			if (prevY < currY)
				Y = currY + 1;
			
			if (prevY != currY)
			{
				if (!ContainsPoint(X, Y) && !PointLaysOnContour(X, Y)					)
					return nullptr;
			}

			if (prevY == currY)
			{
				// Имеем плато минимум из трёх точек prevPoint, pointIndexб nextPoint 
				// В этом случае возвращаем prevPoint или nextPoint в зависимости от
				// требуемого направления поиска и направления изменения координаты Х при 
				// переборе точек в порядке возрастания номеров
				switch (direction)
				{
				case Contour::Left:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[prevPoint] : &m_Points[nextPoint];
				case Contour::Right:
					return (m_Points[prevPoint].X < m_Points[nextPoint].X) ? &m_Points[nextPoint] : &m_Points[prevPoint];
				default:
					break;
				}
			}
		}
		// XPoints содержит список номеров точек с координатами X,Y;
		std::vector<int>* XPoints = (m_PointsMap[currY])->at(m_Points[pointIndex].X);
		if (XPoints->size() > 1)
		{
			return &m_Points[XPoints->at(0)];
		}
		else
		{
			switch (direction)
			{
			case Right:
				return  FindRightNearestPoint(pointIndex);
			case Left:
				return  FindLeftNearestPoint(pointIndex);
			}
		}
		return nullptr;
	}

	/*
	Возвращает индекс следующей точки контура.
	Для последней точки контура следующей считается точка с номером 0
	Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetNextContourPointIndex(int i)
	{
		int k = i + 1;
		return k % Length;
	}

	/*
	 Возвращает индекс предыдущей точки контура
	 Для точки контура с номером 0 предыдущей считается последняя точка контура
	 Отсчёт идёт по часовой стрелке
	*/
	int Contour::GetPrevContourPointIndex(int i)
	{
		int k = i + Length - 1 ;
		return k % Length;
	}

	bool Contour::PointLaysOnContour(int x, int y)
	{
		return (m_PointsMap.count(y) != 0) && (m_PointsMap[y]->count(x) != 0);
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
		//  Если среди точек контура нет точек с координатой Y равной y - точка x, y лежит за пределами контура
		if (m_PointsMap.count(y) == 0)
			return false;
		// Проверим принадлежность точки контуру
		if (m_PointsMap[y]->count(x) != 0)
			return false;
	
		int contourCrossingCount = 0;
		
		for (auto v : *m_PointsMap[y])
		{
			if (v.first > x)
				for (int currentPointIndex : *v.second)
				{
					// Получим индексы точек перед и после текущей точки 
					int prevPointIndex = GetPrevContourPointIndex(currentPointIndex);
					int nextPointIndex = GetNextContourPointIndex(currentPointIndex);

					int currentPointY = m_Points[currentPointIndex].Y;
					// Сравним координаты Y точек соседних с текущей 
					if (m_Points[prevPointIndex].Y == currentPointY)
					{
						// Если предыдущая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
						// по контуру назад пока не найдётся точка лежащая выше или ниже текущей точки.
						while (m_Points[prevPointIndex].Y == currentPointY)
							prevPointIndex = GetPrevContourPointIndex(prevPointIndex);
					}

					if (m_Points[nextPointIndex].Y == currentPointY)
					{
						// Если следующая точка контура лежит на той же горизонтали что текущая точка, начинаем двигаться
						// по контуру вперёд пока не найдётся точка лежащая выше или ниже текущей точки.
						while (m_Points[nextPointIndex].Y == currentPointY)
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
					}
				}
		}
		return contourCrossingCount % 2 == 1 ? true : false;
	}



}  // namespace ContourHelpers
