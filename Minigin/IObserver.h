#ifndef IOBSERVER_H
#define IOBSERVER_H

namespace yev
{
	class GameObject;
	class IObserver
	{
	public:
		virtual ~IObserver() = default;
		using Event = int;
		virtual void Notify(Event event, GameObject* actor) = 0;
	};
}
#endif // !IOBSERVER_H
