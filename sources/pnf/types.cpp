//
// Created by gregorian-rayne on 13/01/2026.
//

#include "pnf/types.hpp"

namespace pnf
{
    const char* pattern_type_to_string(const PatternType type) {
        switch (type) {
        case PatternType::None: return "None";
        case PatternType::DoubleTopBreakout: return "Double Top Breakout";
        case PatternType::DoubleBottomBreakdown: return "Double Bottom Breakdown";
        case PatternType::TripleTopBreakout: return "Triple Top Breakout";
        case PatternType::TripleBottomBreakdown: return "Triple Bottom Breakdown";
        case PatternType::QuadrupleTopBreakout: return "Quadruple Top Breakout";
        case PatternType::QuadrupleBottomBreakdown: return "Quadruple Bottom Breakdown";
        case PatternType::AscendingTripleTop: return "Ascending Triple Top";
        case PatternType::DescendingTripleBottom: return "Descending Triple Bottom";
        case PatternType::BullishCatapult: return "Bullish Catapult";
        case PatternType::BearishCatapult: return "Bearish Catapult";
        case PatternType::BullishSignalReversed: return "Bullish Signal Reversed";
        case PatternType::BearishSignalReversed: return "Bearish Signal Reversed";
        case PatternType::BullishTriangle: return "Bullish Triangle";
        case PatternType::BearishTriangle: return "Bearish Triangle";
        case PatternType::LongTailDown: return "Long Tail Down";
        case PatternType::HighPole: return "High Pole";
        case PatternType::LowPole: return "Low Pole";
        case PatternType::BullTrap: return "Bull Trap";
        case PatternType::BearTrap: return "Bear Trap";
        case PatternType::SpreadTripleTop: return "Spread Triple Top";
        case PatternType::SpreadTripleBottom: return "Spread Triple Bottom";
        default: return "Unknown";
        }
    }

    bool is_bullish_pattern(const PatternType type) {
        return type == PatternType::DoubleTopBreakout ||
               type == PatternType::TripleTopBreakout ||
               type == PatternType::QuadrupleTopBreakout ||
               type == PatternType::AscendingTripleTop ||
               type == PatternType::BullishCatapult ||
               type == PatternType::BullishSignalReversed ||
               type == PatternType::BullishTriangle ||
               type == PatternType::LongTailDown ||
               type == PatternType::LowPole ||
               type == PatternType::BearTrap ||
               type == PatternType::SpreadTripleTop;
    }

    bool is_bearish_pattern(const PatternType type) {
        return type == PatternType::DoubleBottomBreakdown ||
               type == PatternType::TripleBottomBreakdown ||
               type == PatternType::QuadrupleBottomBreakdown ||
               type == PatternType::DescendingTripleBottom ||
               type == PatternType::BearishCatapult ||
               type == PatternType::BearishSignalReversed ||
               type == PatternType::BearishTriangle ||
               type == PatternType::HighPole ||
               type == PatternType::BullTrap ||
               type == PatternType::SpreadTripleBottom;
    }
}