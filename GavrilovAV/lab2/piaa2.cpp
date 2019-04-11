#include <iostream>
#include <map>
#include <queue>
#include <cmath>
#include <string>
#include <algorithm>
#define TEST

class Graph{
	std::map<char,std::map<char,double>> list; 							//список смежности
public:
	void add_way(char from, char to, double length){					//функция добавления пути и вершины в список
#ifdef TEST
		std::cout<<"Добавляем путь из: "<<from<<" в "<<to<<' '<<length<<std::endl;
#endif
		if(from==to)
			return;
		std::pair<char,double> way(to,length);
		if(list.find(from)==list.end()){								//добавляем вершину "из", если ее нет в графе
			std::map<char,double> l;
			l.insert(way);
			list.insert(std::pair<char,std::map<char,double>>(from,l));
			return;
		}
		list[from].insert(way);											//добавляем путь из вершины, если эта вершина уже есть в списке
	}

	double heuristic(char from, char to)								//функция подсчета эвристики
	{
	    return abs(static_cast<long>(from) - static_cast<long>(to));
	}

	double find_way_cost(char from, char to){							//функция, возвращающая длину пути до инцидентной вершины
		if(list.find(from)==list.end())
			return 0.0;
		auto i=list[from].find(to);
		if(i==list[from].end())
			return 0.0;
		return i->second;
	}
	void print_q(std::priority_queue <std::pair<double, char>,
		std::vector < std::pair<double, char> >,
		std::greater< std::pair<double, char>>> q){						//функция печати очереди с приоритетом
		std::cout<<"Queue:\n";
		while(!q.empty()){
			std::cout<<"Вершина: "<<q.top().second<<", эвристика + цена = "<<q.top().first<<std::endl;
			q.pop();
		}
		std::cout<<std::endl;
	}
	double get_cost(std::string way){						//возвращает длину пути без учета эвристики
		double sum=0.0;
		for(auto i=0;i<way.length()-1;i++){
			sum+=find_way_cost(way[i],way[i+1]);
		}
		return sum;
	}

	void is_adm(std::string way){							//функция проверяет на допустимость и монотонность
		bool is_admis=true,is_mon=true;
		char to=*way.rbegin();
		for(auto i=0;i<way.length()-1 && is_admis;i++){
			auto cost=get_cost(way.substr(i));
			if(heuristic(way[i],to)>cost){					//если не допустима
				is_admis=false;
				break;
			}
			if(heuristic(way[i],to)>find_way_cost(way[i],way[i+1])+heuristic(way[i+1],to))  //если не монотонна
				is_mon=false;
		}
		if(is_admis){
			std::cout<<"Функция допустима!\n";
			if(is_mon){
				std::cout<<"Функция монотонна!\n";
			}
			else{
				std::cout<<"Функция не монотонна!\n";
			}
		}
		else
			std::cout<<"Функция не допустима и не монотонна!\n";
	}

	void find_min_way(char from, char to){					//функция поиска наименьшего пути
		std::priority_queue <std::pair<double, char>,
		std::vector < std::pair<double, char> >,
		std::greater< std::pair<double, char>>> q;			//очередь с приоритетом
		q.push({0,from});
#ifdef TEST
		std::cout<<"Начинаем поиск!\n";
#endif
		std::map<char, double> way_cost;					//список пар с минимально ценой до просмотренных вершин
		std::map<char,char> point_hist;						//список путей текущего результирующего путя
		while(!q.empty()){									//пока список не пуст
			auto cur = q.top();
			q.pop();
			if(cur.second==to){								//если найден конец
#ifdef TEST
				std::cout<<"Конец достигнут!\n";
#endif
				break;
			}
#ifdef TEST
			std::cout<<"Ищем пути из: "<<cur.second<<std::endl;
#endif
			for(auto i=list[cur.second].begin();i!=list[cur.second].end();i++){								//просматриваем все инцидентные вершины
#ifdef TEST
				std::cout<<"Просматриваем путь: "<<cur.second<<"->"<<i->first<<" за "<<i->second<<std::endl;
#endif
				double new_cost=way_cost[cur.second]+find_way_cost(cur.second,i->first);					//считаем оценку до вершины
				if(!way_cost.count(i->first) || new_cost<way_cost[i->first]){								//если в вершину еще не ходили, либо найден путь меньше предыдущего
#ifdef TEST
					std::cout<<"Добавляем вершину в очередь!\n";
#endif
					way_cost[i->first]=new_cost;
					std::cout<<cur.second<<"->"<<i->first<<' '<< new_cost<<std::endl;
					q.push({new_cost+heuristic(i->first,to),i->first});										//опускаем в очередь
					point_hist[i->first]=cur.second;
				}
			}
#ifdef TEST
				print_q(q);
#endif

		}
		std::string answer;
		answer.push_back(to);																		//печать ответа
		while (answer.back() != from)
			answer.push_back(point_hist[answer.back()]);
		std::reverse(answer.begin(), answer.end());
		is_adm(answer);
		std::cout<<"Резульат: \n";
		for (auto e : answer)
			std::cout << e;
		std::cout<<std::endl;
	}

};

int main(){
	Graph a;
	char start,end,from,to;
	std::cin>>start>>end;
	double length;
	while(std::cin>>from){
		std::cin>>to>>length;
		a.add_way(from, to, length);
	}
	a.find_min_way(start,end);
	return 0;
}
