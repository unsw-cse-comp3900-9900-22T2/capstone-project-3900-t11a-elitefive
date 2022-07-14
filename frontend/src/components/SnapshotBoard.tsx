import React, { useEffect, useState } from 'react';
import styled from 'styled-components';

// local
import HexCell from './HexCell';


type Props = {
  width?: number;
  height?: number;
  isStatic?: boolean;
}

type RowStyleProps = {
  x_offset: number
}

const defaultBoard = [
  ['a1', 'a2', 'a3', 'a4', 'a5'],
  ['b1', 'b2', 'b3', 'b4', 'b5', 'b6'],
  ['c1', 'c2', 'c3', 'c4', 'c5', 'c6', 'c7'],
  ['d1', 'd2', 'd3', 'd4', 'd5', 'd6', 'd7', 'd8'],
  ['e1', 'e2', 'e3', 'e4', 'e5', 'e6', 'e7', 'e8', 'e9'],
  ['f1', 'f2', 'f3', 'f4', 'f5', 'f6', 'f7', 'f8'],
  ['g1', 'g2', 'g3', 'g4', 'g5', 'g6', 'g7'],
  ['h1', 'h2', 'h3', 'h4', 'h5', 'h6'],
  ['i1', 'i2', 'i3', 'i4', 'i5'],
]

const BoardContainer = styled.div`
  padding: 50px;
`;

// horizontal
const RowContainer = styled.span<RowStyleProps>`
  margin-left: ${props => props.x_offset}px;
  display: flex;
`;

// vertical
const ColumnContainer = styled.div`
  position: relative;
  margin-top: -15px;
`;

const DEFAULT_BOARD_WIDTH = 1200;
// const DEFAULT_BOARD_HEIGHT = 800;

export default function SnapshotBoard({ width, height, isStatic }: Props) {
  
  const renderRow = (row: string[] , x_offset: number) => {
    return (
      <RowContainer x_offset={x_offset}>
        {row.map(( _x : string) => {
          return (
            <HexCell
              width={60}
              height={65}
              // fill={hexState?.color ? hexState?.color : "var(--accent-darker)"}
              onClick={() => {
                // if isStatic does not do any of this Websocket stuff
                if(!isStatic) {
                  // playMove("abc", hexKey);
                  // then sends WS emit method which will cause change
                }
              }}
            />
          )
        })}
      </RowContainer>
    )
  }

  const renderBoard = () => {
    const w = width ? width : DEFAULT_BOARD_WIDTH;

    return defaultBoard.map((row, index) => {
      // it works
      const x_offset = Math.abs((index+1)- 5) * w/40
      const y_offset = index * 90;
      return (
        <ColumnContainer>
          {
            renderRow(row, x_offset)
          }
        </ColumnContainer>
      )
    })
  
  }

  return (
    <BoardContainer>
      {renderBoard()}
    </BoardContainer>
  )
}