package com.alibaba;

public class Java003IFDemo {

	public static void main(String[] args) {
		// IF�ж����
		int num = 9000;
		if (num >= 10000000) {
			System.out.println("������������Ա");
		} else if (num >= 1000000) {
			System.out.println("��������Ա");
		}
		else if (num >= 100000) {
			System.out.println("�в�����Ա");
		}
		else if (num >= 10000) {
			System.out.println("��ũ");
		}
		else {
		      System.out.println("����");
		}
	}

}
