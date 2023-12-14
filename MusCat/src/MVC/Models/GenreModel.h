#pragma once
#include <orm/tiny/model.hpp>


class MGenre final : public Orm::Tiny::Model<MGenre>
{
	friend Orm::Tiny::Model;

private:
	QString u_table{ "genre" };
};