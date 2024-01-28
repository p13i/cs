
#if 0

enum class Person {
    Id = 0;
    Name = 1;
    Age = 2;
    Address = 3;
};



class Table<DataTypeEnum> {
    DataTypeEnum row() {
        return DataTypeEnum();
    }
}


class PeopleTable : public Table<Person> {
};

#define WHERE(cond) [](tab) { return tab.Age > 0; }

const auto age_gt_0 = [](PeopleTable table) { return table.row()::Age >= 55; }

const Query get_all_people_in_retirement_age = table.query()
    .select("*")
    .from("MySqlTable")
    .where(age_gt_0);
Result<std::vector<People>> result = get_all_people_in_retirement_age.excute();
ENSURE_OK(result);
std::vector<People> people_able_to_retire = result.data();

#endif
