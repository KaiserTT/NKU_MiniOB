#pragma once

#include <vector>
#include "sql/operator/physical_operator.h"
#include "sql/parser/parse_defs.h"

class Trx;
class UpdateStmt;

/**
 * @brief update physical operator
 * @ingroup PhysicalOperator
*/
class UpdatePhysicalOperator : public PhysicalOperator
{
public:
    UpdatePhysicalOperator(Table* table, FieldMeta& field, Value& value) : table_(table), field_(field), value_(value) {}
    virtual ~UpdatePhysicalOperator() = default;

    PhysicalOperatorType type() const override
    {
        return PhysicalOperatorType::UPDATE;
    }

    RC open(Trx* trx) override;
    RC next() override;
    RC close() override;

    Tuple* current_tuple() override { return nullptr; }

private:
    Table* table_ = nullptr;
    FieldMeta field_;
    Value value_;
    Trx* trx_ = nullptr;
};