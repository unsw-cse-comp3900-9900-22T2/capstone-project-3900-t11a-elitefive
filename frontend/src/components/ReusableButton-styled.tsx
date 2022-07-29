import styled from "styled-components";

export type buttonProps = {
  background?: string;
  width?: number;
  height?: number;
  color?: string;
}


export const StyledButton = styled.div<buttonProps>`
  width: ${props=>props.width?props.width:"120"}px;
  height: ${props=>props.height?props.height+"px":"auto"};
  padding: 10px 0;
  background: ${props=>props.background?props.background:"var(--textbox-dark)"};
  color: ${props=>props.color?props.color:"white"};
  display: flex;
  justify-content: center;
  align-items: center;
  border-radius: 10px;
  &:hover {
    background: #EFEEF3;
    color: black;
    transform: scale(1.04);
  }
  &:active {
    background: #C8D1FA;
    color: #7482CB;
    transform: scale(0.96);
  }
  cursor: pointer;
`;

export const StyledLargeButton = styled.div<buttonProps>`
  width: ${props=>props.width?props.width:"350"}px;
  height: ${props=>props.height?props.height:"400"}px;
  background: ${props=>props.background?props.background:"var(--textbox-dark)"};
  color: ${props=>props.color?props.color:"white"};
  display: flex;
  justify-content: center;
  align-items: center;
  border-radius: 10px;
  &:hover {
    background: #814bcb;
    color: white;
    transform: scale(1.04);
  }
  &:active {
    background: #C8D1FA;
    color: #814bcb;
    transform: scale(0.96);
  }
  cursor: pointer;
`;