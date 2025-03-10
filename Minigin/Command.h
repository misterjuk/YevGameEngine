
#ifndef COMMAND_H
#define COMMAND_H

namespace yev
{
	class Command {
	public:
		virtual ~Command() = default;
		virtual void Execute() = 0;
	};
}

#endif 