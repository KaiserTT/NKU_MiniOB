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
        LOG_WARN("aggregate operator must have one and only one child");
        return RC::INTERNAL;
    }

    return children_[0]->open(trx);
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
                rc = tuple->cell_at(cell_idx, cell);
                attr_type = cell.attr_type();
                if (attr_type == AttrType::INTS or attr_type == AttrType::FLOATS) {
                    result_cells[cell_idx].set_float(result_cells[cell_idx].get_float() + cell.get_float());
                }
                break;
            default:
                return RC::UNIMPLENMENT;
            }
        }
    }
    if (rc == RC::RECORD_EOF) {
        rc = RC::SUCCESS;
    }

    result_tuple_.set_cells(result_cells);

    return rc;
}

RC AggregatePhysicalOperator::close() 
{
    return children_[0]->close();
}