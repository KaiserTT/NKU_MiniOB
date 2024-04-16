#include "sql/operator/aggregate_physical_operator.h"
#include "common/log/log.h"
#include "storage/record/record.h"
#include "storage/table/table.h"
#include "storage/trx/trx.h"
#include "sql/stmt/select_stmt.h"

void AggregatePhysicalOperator::add_aggregation(const AggrOp aggregation)
{
    aggregations_.push_back(aggregation);
}

RC AggregatePhysicalOperator::open(Trx *trx)
{
    if (children_.size() != 1) {
        LOG_WARN("aggregate operator must have one child");
        return RC::INTERNAL;
    }

    std::unique_ptr<PhysicalOperator> &child = children_[0];
    RC rc = child->open(trx);
    if (rc != RC::SUCCESS) {
        LOG_WARN("failed to open child operator: %s", strrc(rc));
        return rc;
    }

    return RC::SUCCESS;
}

RC AggregatePhysicalOperator::next()
{
    //already aggregated
    if (result_tuple_.cell_num() > 0) {
        return RC::RECORD_EOF;
    }

    RC rc = RC::SUCCESS;
    PhysicalOperator *oper = children_[0].get();

    // do aggregate
    int count = 0;
    std::vector<Value> result_cells;
    while (RC::SUCCESS == (rc = oper->next())) {
        // get tuple
        Tuple *tuple = oper->current_tuple();

        for (int cell_idx = 0; cell_idx < (int)aggregations_.size(); cell_idx++) {
            const AggrOp aggregation = aggregations_[cell_idx];

            Value cell;
            AttrType attr_type = AttrType::INTS;
            switch (aggregation)
            {
            case AggrOp::AGGR_SUM:
            case AggrOp::AGGR_AVG:
                rc = tuple->cell_at(cell_idx, cell);
                attr_type = cell.attr_type();
                if (result_cells.size() == 0) {
                    result_cells.push_back(Value(0.0f));
                }
                if (attr_type == AttrType::INTS or attr_type == AttrType::FLOATS) {
                    result_cells[cell_idx].set_float(result_cells[cell_idx].get_float() + cell.get_float());
                }
                break;
            default:
                return RC::UNIMPLENMENT;
            }
        }
        count++; 
    }
    if (rc == RC::RECORD_EOF) {
        rc = RC::SUCCESS;
    }

    for (int cell_idx = 0; cell_idx < result_cells.size(); cell_idx++) {
        const AggrOp aggregation = aggregations_[cell_idx];

        if (aggregation == AggrOp::AGGR_AVG)
            result_cells[cell_idx].set_float(result_cells[cell_idx].get_float() / count);
    }

    result_tuple_.set_cells(result_cells);

    return rc;
}

RC AggregatePhysicalOperator::close() 
{
    return children_[0]->close();
}

Tuple *AggregatePhysicalOperator::current_tuple() 
{
    return &result_tuple_;
}
