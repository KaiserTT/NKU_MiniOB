/* Copyright (c) 2021 OceanBase and/or its affiliates. All rights reserved.
miniob is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL v2.
You may obtain a copy of Mulan PSL v2 at:
         http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

//
// Created by WangYunlai on 2022/07/05.
//

#include <sstream>
#include "sql/expr/tuple_cell.h"
#include "storage/field/field.h"
#include "common/log/log.h"
#include "common/lang/comparator.h"
#include "common/lang/string.h"

TupleCellSpec::TupleCellSpec(const char *table_name, const char *field_name, const char *alias, const AggrOp aggr)
{
  if (table_name) {
    table_name_ = table_name;
  }
  if (field_name) {
    field_name_ = field_name;
  }

  aggr_ = aggr;
  if (alias) {
    alias_ = alias;
  } else {
    if (table_name_.empty()) {
      alias_ = field_name_;
    } else {
      alias_ = table_name_ + "." + field_name_;
    }

    if (aggr_ == AggrOp::AGGR_COUNT_ALL) {
      alias_ = "COUNT(*)";
    } else if (aggr_ != AggrOp::AGGR_COUNT_ALL && aggr_ != AggrOp::AGGR_NONE) {
      std::string aggr_str;
      aggr_to_string(aggr_, aggr_str);
      alias_ = aggr_str + "(" + alias_ + ")";
    }
  }
}

TupleCellSpec::TupleCellSpec(const char *alias, const AggrOp aggr = AGGR_NONE)
{
  aggr_ = aggr;
  if (alias) {
    alias_ = alias;
    if (aggr_ == AggrOp::AGGR_COUNT_ALL) {
      alias_ = "COUNT(*)";
    } else if (aggr_ != AggrOp::AGGR_COUNT_ALL && aggr_ != AggrOp::AGGR_NONE) {
      std::string aggr_str;
      aggr_to_string(aggr_, aggr_str);
      alias_ = aggr_str + "(" + alias_ + ")";
    }
  }
}

void TupleCellSpec::aggr_to_string(const AggrOp aggr, std::string& aggr_str) {
  switch (aggr)
  {  
  case AGGR_SUM:
    aggr_str = "SUM";
    break;
  case AGGR_AVG:
    aggr_str = "AVG";
    break;
  case AGGR_MAX:
    aggr_str = "MAX";
    break;
  case AGGR_MIN:
    aggr_str = "MIN";
    break;
  case AGGR_COUNT:
    aggr_str = "COUNT";
  default:
    break;
  }
}