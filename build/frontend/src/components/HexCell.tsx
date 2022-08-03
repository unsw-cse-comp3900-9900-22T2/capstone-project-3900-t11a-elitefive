import React from 'react'
import styled from 'styled-components';

type Props = {
  width?: number;
  height?: number;
  stroke?: string;
  fill?: string;
  opacity?: number;
  transform?: string;
  onClick?: (args?: any) =>void;
}

type StyledProps = {
  width?: number;
  height?: number;
  stroke?: string;
  fill?: string;
  opacity?: number;
}

// const DEFAULT_WIDTH = 110;
// const DEFAULT_HEIGHT = 120;

const DEFAULT_WIDTH = 100;
const DEFAULT_HEIGHT = 110;

const DEFAULT_STROKE = "#7a7a7a"  // Hex outline
const DEFAULT_FILL = "#2b3135"
const DEFAULT_TRANSFORM = ""
const DEFAULT_STROKE_WIDTH = 3;
const DEFAULT_HOVER_FILL = "#575757"; // Hover colour
const DEFAULT_OPACITY = "100";


const SVGContainer = styled.svg<StyledProps>`
  width: ${props => props.width? props.width : DEFAULT_WIDTH}px;
  height: ${props => props.height? props.height: DEFAULT_HEIGHT}px;
  opacity: ${props => props.opacity? props.opacity : DEFAULT_OPACITY}px;

  & > polygon {
    stroke: ${props => props.stroke? props.stroke : DEFAULT_STROKE};
    fill: ${props => props.fill? props.fill : DEFAULT_FILL};
    stroke-width: ${DEFAULT_STROKE_WIDTH};
  }

  & > polygon:hover {
    fill: ${DEFAULT_HOVER_FILL};
  }
`;

export default function HexCell(props: Props) {
  const { 
    width,
    height,
    stroke,
    fill,
    opacity,
    transform,
    onClick,
  } = props;

  // generates path for a perfect hexagon
  const getPathCoords = () => {
    const w = width ? width : DEFAULT_WIDTH;
    const h = height ? height: DEFAULT_HEIGHT;
    const a = [w/2, 0];
    const b = [w, h/4];
    const c = [w, h * 3/4];
    const d = [w/2, h];
    const e = [0, h * 3/4];
    const f = [0, h / 4];
  
    const anchors = [a,b,c,d,e,f];
    let pathString = "";
    for(const tuple of anchors) {
      const tupleAsString = tuple[0]+","+tuple[1];
      pathString += tupleAsString;
      pathString += " ";
    }
    pathString.trim();
    return pathString;
  }

  return (
    <SVGContainer
      width={width} 
      height={height}
      opacity={opacity}
      onClick={onClick}
      fill={fill}
      stroke={stroke}
    >
      <polygon
        points={getPathCoords()}
        transform={transform? transform: DEFAULT_TRANSFORM}
      />
    </SVGContainer>
  )
}