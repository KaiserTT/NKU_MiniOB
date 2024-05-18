#pragma once

#include <vector>

#include "sql/operator/logical_operator.h"
#include "sql/parser/parse_defs.h"

/**
 * @brief update logical operator
 * @ingroup LogicalOperator
*/

class UpdateLogicalOperator : public LogicalOperator
{
public:
    UpdateLogicalOperator(Table* table, FieldMeta field, Value value);
    virtual ~UpdateLogicalOperator() = default;

    LogicalOperatorType type() const override
    {
        return LogicalOperatorType::UPDATE;
    }

    Table *table() const { return table_; }

    FieldMeta& field() { return field_; }

    Value& value() { return value_; }

private:
    Table* table_ = nullptr;
    FieldMeta field_;
    Value value_;
};