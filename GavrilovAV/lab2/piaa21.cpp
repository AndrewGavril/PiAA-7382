#include <iostream>
#include <map>
#include <list>
#include <cmath>
#include <string>
#include <algorithm>

#define TEST
class Graph{
	bool find_point(std::list<char>& points_list, char point){				//функция поиска вершины в листе
		for(auto i=points_list.begin();i!=points_list.end();i++){
			if(*i==point)
				return true;
		}
		return false;
	}
	std::map<char,std::map<char,double>> list;								//список смежности
public:
	void add_way(char from, char to, double length){							//добавляем путь 
#ifdef TEST
		std::cout<<"Добавляем путь из: "<<from<<" в "<<to<<' '<<length<<std::endl;
#endif
		if(from==to)
			return;
		std::pair<char,double> way(to,length);
		if(list.find(from)==list.end()){										//если вершины нет в графе, то добавляем ее
			std::map<char,double> l;
			l.insert(way);
			list.insert(std::pair<char,std::map<char,double>>(from,l));
			//std::cout<<list.begin()->first<<' '<<(list[from]).begin()->first<<std::endl;
			return;
		}
		list[from].insert(way);													//если вершина уже есть, то добавляем только путь
	}
	double find_way_cost(char from, char to){									//функция поиска длины пути до инцидентной вершины
		if(list.find(from)==list.end())
			return -1.0;
		auto i=list[from].find(to);
		if(i==list[from].end())
			return -1.0;
		return i->second;
	}
	bool has_any_way(std::list<char> &points_hist, std::list<char> &deadlocks, char point){		//проверяет наличие путей из вершины, ведущих не в посещенную вершину и не в тупик
		for(auto i=list[point].begin();i!=list[point].end();i++){
			if(!find_point(points_hist,i->first) && !find_point(deadlocks,i->first))
				return true;
		}
		return false;
	}
	void find_min_way(char from, char to){								//функция поиска наименьшего пути 
		std::list<char> points_hist, deadlocks;							//список с текущим путем и список тупиков
		char cur=from,next;
		double min;
		points_hist.push_front(cur);
#ifdef TEST
		std::cout<<"Начинаем поиск!\n";
#endif							
		while(cur!=to){													//пока не найден конец
#ifdef TEST
			std::cout<<"Ищем пути из: "<<cur<<std::endl;
#endif
			bool flag=true;
			if(!has_any_way(points_hist,deadlocks,cur)){				//если есть путь, то продолжаем, иначе возвращаемся в предыдущую вершину и ищем другие пути
				//std::cout<<cur<<" тупик\n";
#ifdef TEST
				std::cout<<"Найден тупик, возвращаемся на шаг назад и добавляем вершину в список тупиков!\n";
#endif
				deadlocks.push_back(cur);
				points_hist.pop_back();
				cur=points_hist.back();
				continue;
			}
			for(auto i=list[cur].begin();i!=list[cur].end();i++){							//просматриваем инцидентные вершины
#ifdef TEST
				std::cout<<"Просматриваем путь: "<<cur<<"->"<<i->first<<" за "<<i->second<<std::endl;
#endif
				if(flag && !find_point(points_hist,i->first) && !find_point(deadlocks,i->first)){		//устанавливаем минимум, равный первому доступному пути
					min=i->second;
					next=i->first;
					flag=false;
					continue;
				}
				if(i->second<min && !find_point(points_hist,i->first) && !find_point(deadlocks,i->first)){ //если найден довое минимальное ребро
#ifdef TEST
				std::cout<<"Следующая вершина: "<<i->first<<std::endl;
#endif
					next=i->first;
				}
			}
			cur=next;
			points_hist.push_back(cur);
#ifdef TEST
			std::cout<<"Текущий путь: ";
		for(auto i=points_hist.begin();i!=points_hist.end();i++){
				std::cout<<*i;
		}
		std::cout<<std::endl<<std::endl;

#endif
		}
		std::cout<< "Результат: ";
		for(auto i=points_hist.begin();i!=points_hist.end();i++){			//печать результата
			std::cout<<(*i);
		}
		std::cout<<std::endl;
	}


};

int main(){
	Graph a;
	char start,end,from,to;
	std::cin>>start>>end;
	double length;
	while(std::cin>>from>>to>>length){
		a.add_way(from, to, length);
	}
	a.find_min_way(start,end);
	return 0;
}