import styled from "styled-components";

export type cardProps = {
    background?: string;
    width?: number;
    height?: number;
    color?: string;
}

export const StyledCard = styled.div<cardProps>`
  width: ${props=>props.width?props.width:"350"}px;
  background: ${props=>props.background?props.background:"var(--textbox-dark)"};
  color: ${props=>props.color?props.color:"white"};
  display: flex;
  justify-content: center;
  align-items: center;
  border-radius: 10px;
`;
