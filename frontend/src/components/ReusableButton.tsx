import React from 'react';
import styled from 'styled-components';
import { StyledButton, StyledLargeButton, buttonProps } from './ReusableButton-styled';
import { TextField, Typography } from '@mui/material'; 



type Props = {
  children?: React.ReactElement | any;
  onClick?: (...args: any) => void;
} & buttonProps;

const Container = styled.div`
  display:flex;
  flex-direction: column;
`;

 

export default function Button({ children, onClick, ...styleProps }: Props) {
  return (
    <StyledButton
      onClick={onClick}
      {...styleProps}
    >
      <Typography variant="subtitle1">{children}</Typography>
    </StyledButton>
  );
}

export function Button2({ children, onClick, ...styleProps }: Props) {
    return (
      <StyledButton
        onClick={onClick}
        {...styleProps}
      >
        <Container>
            {children}
        </Container>
      </StyledButton>
    );
}

export function LargeButton({ children, onClick, ...styleProps }: Props) {
    return (
      <StyledLargeButton
        onClick={onClick}
        {...styleProps}
      >
        <Container>
            {children}
        </Container>
      </StyledLargeButton>
    );
}
